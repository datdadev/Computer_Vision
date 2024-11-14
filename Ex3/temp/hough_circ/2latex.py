import json

# Load JSON data from coordinates.json
with open('coordinates.json', 'r') as file:
    data = json.load(file)

# Create LaTeX table code
latex_output = []

for key, angles in data.items():
    multirow_line = f"\\multirow{{{len(angles)}}}{{*}}[0pt]{{$ {key} $}} &"
    for angle, radius_data in angles.items():
        row = f" ${angle}$"
        for radius in sorted(radius_data.keys()):
            xc = radius_data[radius]['xc']
            yc = radius_data[radius]['yc']
            vote = radius_data[radius]['vote']

            color = r"\cellcolor{TransparentYellow}" if vote >= 3 else ""
            row += f" & {color} ${xc}$ & {color} ${yc}$ & {color} {vote}"

        latex_output.append(multirow_line + row + " \\\\\\cline{2-17}")
        multirow_line = "&"  # Change for subsequent lines

    latex_output.append("\\hline")

# Join all lines
latex_table = "\n".join(latex_output)

# Save the output to a .txt file
with open('latex_table.txt', 'w') as output_file:
    output_file.write(latex_table)

print("LaTeX table saved to latex_table.txt")
