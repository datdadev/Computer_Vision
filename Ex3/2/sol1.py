import cv2
import numpy as np

def main():
    src = cv2.imread("./image.jpg")
    if src is None:
        print("Error loading image!")
        return

    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 1.5)
    edges = cv2.Canny(blurred, 50, 150)

    lines = cv2.HoughLines(edges, 1, np.pi / 180, 240)

    # Draw detected lines on the image
    # if lines is not None:
    #     for line in lines:
    #         rho, theta = line[0]
    #         a = np.cos(theta)
    #         b = np.sin(theta)
    #         x0 = a * rho
    #         y0 = b * rho
            
    #         pt1 = (int(x0 + 1000 * (-b)), int(y0 + 1000 * (a)))
    #         pt2 = (int(x0 - 1000 * (-b)), int(y0 - 1000 * (a)))
            
    #         cv2.line(src, pt1, pt2, (0, 255, 0), 1, cv2.LINE_AA)

    x = []
    y = []
    for line in lines:
        rho = line[0][0]
        theta = line[0][1]

        if theta == 0: x.append(rho * np.cos(theta))
        else: y.append(rho * np.sin(theta))

    def average_pairs_within_distance(values, distance):
        values.sort()
        averaged_values = []
        
        i = 0
        while i < len(values):
            current_value = values[i]
            total = current_value
            count = 1

            j = i + 1
            while j < len(values) and np.abs(values[j] - current_value) < distance:
                total += values[j]
                count += 1
                j += 1

            averaged_values.append(total / count)

            i = j

        return averaged_values

    x_main = average_pairs_within_distance(x, 15)
    y_main = average_pairs_within_distance(y, 15)
    
    width = src.shape[1]
    height = src.shape[0]

    # for x_val in x_main:
    #     cv2.line(src, (int(x_val), 0), (int(x_val), width), (0, 0, 255), 1)  # vertical line

    # for y_val in y_main:
    #     cv2.line(src, (0, int(y_val)), (height, int(y_val)), (255, 0, 0), 1)  # horizontal line

    boxes = []
    for i in range(len(y_main)):
        for j in range(0, len(x_main) - 1, 1):
            if i == 0: boxes.append([[x_main[j], 0], [x_main[j+1], y_main[i]]])
            else: boxes.append([[x_main[j], y_main[i]], [x_main[j+1], height]])

    num_slots = str(len(boxes))
    print("Number of parking slots: " + num_slots)

    dst1 = src.copy()

    for idx, box in enumerate(boxes):
        top_left = (int(box[0][0]), int(box[0][1]))  # top-left corner
        bottom_right = (int(box[1][0]), int(box[1][1]))  # bottom-right corner

        cv2.rectangle(dst1, top_left, bottom_right, (255, 0, 0), 3)

        center_x = (top_left[0] + bottom_right[0]) // 2
        center_y = (top_left[1] + bottom_right[1]) // 2

        text = str(idx + 1)
        text_size, _ = cv2.getTextSize(text, cv2.FONT_HERSHEY_SIMPLEX, 1, 2)
        text_x = center_x - text_size[0] // 2
        text_y = center_y + text_size[1] // 2

        if idx < len(boxes) / 2:
            cv2.putText(dst1, str(idx + 1), (text_x, 50),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
        else:
            cv2.putText(dst1, str(idx + 1), (text_x, height - 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)
            
    # --------------------------------------------------

    _, thresh = cv2.threshold(blurred, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    kernel_erode = np.ones((5, 5), np.uint8)
    kernel_dilate = np.ones((3, 3), np.uint8)
    eroded = cv2.erode(thresh, kernel_erode, iterations=2)
    dilated = cv2.dilate(eroded, kernel_dilate, iterations=3)

    edges = cv2.Canny(dilated, 50, 150)
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    dst2 = src.copy()
    occupied = []  # array to hold the parked car positions

    for contour in contours:
        # Close the contour if not already closed
        if not np.array_equal(contour[0], contour[-1]):
            contour = np.append(contour, [contour[0]], axis=0)

        area = cv2.contourArea(contour)
        if area > 3000:
            M = cv2.moments(contour)
            if M['m00'] != 0:
                center_x = int(M['m10'] / M['m00'])
                center_y = int(M['m01'] / M['m00'])

                for idx, box in enumerate(boxes):
                    top_left = (int(box[0][0]), int(box[0][1]))  # top-left corner
                    bottom_right = (int(box[1][0]), int(box[1][1]))  # bottom-right corner
                    
                    # Check if the center point is inside the box
                    if (top_left[0] <= center_x <= bottom_right[0] and
                        top_left[1] <= center_y <= bottom_right[1]):
                        occupied.append(idx+1)
                        cv2.circle(dst2, (center_x, center_y), 5, (255, 0, 0), -1)
                        break

                cv2.drawContours(dst2, [contour], -1, (0, 255, 0), 2)

    total_slots = set(range(1, len(boxes) + 1))
    occupied_set = set(occupied)
    available = sorted(total_slots - occupied_set)

    print("Occupied slots:", sorted(occupied))
    print("Available slots:", available)

    dst3 = src.copy()
    offset_x = 3
    offset_y = 3

    for idx, box in enumerate(boxes):
        top_left = (int(box[0][0]) + offset_x, int(box[0][1]) + offset_y)
        bottom_right = (int(box[1][0]) - offset_x, int(box[1][1]) - offset_y)

        status_color = (0, 0, 255) if (idx+1) in occupied else (0, 255, 0)

        cv2.rectangle(dst3, top_left, bottom_right, status_color, 2)

        center_x = (top_left[0] + bottom_right[0]) // 2
        center_y = (top_left[1] + bottom_right[1]) // 2

        text = str(idx + 1)
        text_size, _ = cv2.getTextSize(text, cv2.FONT_HERSHEY_SIMPLEX, 1, 2)
        text_x = center_x - text_size[0] // 2
        text_y = center_y + text_size[1] // 2

        if idx < len(boxes) / 2:
            cv2.putText(dst3, text, (text_x, 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, status_color, 2, cv2.LINE_AA)
        else:
            cv2.putText(dst3, text, (text_x, height - 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, status_color, 2, cv2.LINE_AA)

    cv2.imshow("Result", dst3)

    cv2.imshow("Car Blobs", dst2)

    cv2.imshow("Parking Area", dst1)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
