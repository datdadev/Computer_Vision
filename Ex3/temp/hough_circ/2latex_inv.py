import json

# Load JSON data from coordinates.json
with open('coordinates_reorganized.json', 'r') as file:
    data = json.load(file)

# Create LaTeX table code
latex_output = []

for key, angles in data.items():
    # Create a multirow line for the main key with the number of radii as rows
    multirow_line = f"\\multirow{{{len(angles)}}}{{*}}[0pt]{{$ {key} $}} &"
    angle_keys = list(angles.keys())  # Get angle keys as list to track the last element

    for idx, angle in enumerate(angle_keys):
        radius_data = angles[angle]
        row = f" ${angle[-1]}$"

        # Sort radii to maintain consistent order
        for radius in sorted(radius_data.keys()):
            xc = radius_data[radius]['xc']
            yc = radius_data[radius]['yc']
            vote = radius_data[radius]['vote']

            # Apply color if vote >= 3
            color = r"\cellcolor{TransparentYellow}" if vote >= 3 else ""
            row += f" & {color}${xc}$ & {color} ${yc}$ & {color} {vote}"

        # Check if it's the last index for the current key to avoid unnecessary cline
        if idx < len(angle_keys) - 1:
            row += " \\\\\\cline{2-16}"
        else:
            row += " \\\\"

        latex_output.append(multirow_line + row)
        multirow_line = "&"  # Change for subsequent lines

    latex_output.append("\\hline")

# Join all lines
latex_table = "\n".join(latex_output)

# Save the output to a .txt file
with open('latex_table_reorganized.txt', 'w') as output_file:
    output_file.write(latex_table)

print("LaTeX table saved to latex_table.txt")
