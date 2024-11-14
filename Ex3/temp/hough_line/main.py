import numpy as np

# Given coordinates
coordinates = [
    (4, 10), (5, 10), (6, 10), (7, 10), (8, 10), (9, 10),
    (5, 11), (6, 11), (9, 11), (5, 12), (6, 12), (9, 12),
    (5, 13), (6, 13), (7, 13), (8, 13), (9, 13), (2, 14),
    (4, 14), (5, 14), (6, 14), (7, 14), (8, 14), (9, 14),
    (2, 15), (3, 15), (5, 15), (9, 15), (3, 16), (4, 16),
    (4, 17), (5, 17)
]

# Given theta values in radians
theta_values = np.radians([0, 45, 90, 135, 180])
angles = [0, 45, 90, 135, 180]  # Degree representation for easy reference

# Function to format numbers without ".0" if they are whole numbers
def format_number(num):
    return f"{int(num)}" if num.is_integer() else f"{num:.4f}"

# Calculate and print rho values in LaTeX table format
for x, y in coordinates:
    # Calculate rho values
    rho_values = [x * np.cos(theta) + y * np.sin(theta) for theta in theta_values]
    rho_values = [format_number(round(rho, 4)) for rho in rho_values]
    
    # Print in LaTeX format
    print(f"$({format_number(x)},{format_number(y)})$ & " + " & ".join(f"${rho}$" for rho in rho_values) + " \\\\\hline")

    
