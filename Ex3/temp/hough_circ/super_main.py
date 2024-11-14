import numpy as np

matrix = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
]

coordinates = []

for y in range(len(matrix)):
    for x in range(len(matrix[y])):
        if matrix[y][x] == 1:
            coordinates.append((x + 1, y + 1))

theta_values = np.radians([-90, 0, 90, 180])
r_values = [2, 3]

previous_coords = {}

for xi, yi in coordinates:
    for ri in r_values:
        for theta_rad in theta_values:
            xc = round(xi - ri * np.cos(theta_rad))
            yc = round(yi - ri * np.sin(theta_rad))
            vote_key = (xc, yc, ri)
            
            if vote_key not in previous_coords:
                previous_coords[vote_key] = 0
            
            previous_coords[vote_key] += 1

max_votes = {}

for (xc, yc, ri), vote in previous_coords.items():
    if (xc, yc) not in max_votes:
        max_votes[(xc, yc)] = (vote, ri)
    else:
        if vote > max_votes[(xc, yc)][0]:
            max_votes[(xc, yc)] = (vote, ri)

filtered_results = [{"h": xc, "k": yc, "vote": vote, "radius": ri} for (xc, yc), (vote, ri) in max_votes.items() if vote >= 3]

circle_equations = []
for result in filtered_results:
    h = result["h"]
    k = result["k"]
    r = result["radius"]
    equation = f"(x - {h})^2 + (y - {k})^2 = {r}^2"
    circle_equations.append(equation)

print("Number of circles: " + str(len(circle_equations)))
for i, equation in enumerate(circle_equations):
    print(f"{i + 1:2}: {equation}")
