import numpy as np
from collections import defaultdict

# Given coordinates
coordinates = [
    (4, 10), (5, 10), (6, 10), (7, 10), (8, 10), (9, 10),
    (5, 11), (6, 11), (9, 11), (5, 12), (6, 12), (9, 12),
    (5, 13), (6, 13), (7, 13), (8, 13), (9, 13), (2, 14),
    (4, 14), (5, 14), (6, 14), (7, 14), (8, 14), (9, 14),
    (2, 15), (3, 15), (5, 15), (9, 15), (3, 16), (4, 16),
    (4, 17), (5, 17)
]

# Theta values in radians and their angles in degrees
theta_values = np.radians([0, 45, 90, 135, 180])
angles = [0, 45, 90, 135, 180]

# Function to format numbers without ".0" if they are whole numbers
def format_number(num):
    return int(num) if num.is_integer() else round(num, 4)

# Initialize a dictionary to store votes for each theta and rho
votes_by_theta = {angle: defaultdict(int) for angle in angles}

# Calculate and accumulate votes
for x, y in coordinates:
    for angle, theta in zip(angles, theta_values):
        rho = format_number(x * np.cos(theta) + y * np.sin(theta))
        votes_by_theta[angle][rho] += 1

# Count lines with votes >= 5
line_count = 0
for angle in angles:
    for rho, vote in votes_by_theta[angle].items():
        if vote >= 5:
            line_count += 1
            print(f"Angle: {angle}°, ρ: {rho}, Votes: {vote}")

print(f"\nTotal number of lines with votes >= 5: {line_count}")
