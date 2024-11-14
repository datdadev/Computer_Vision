import json
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

# Given theta and r values
theta_values = np.radians([-90, 0, 90, 180])  # Convert to radians
r_values = [2, 3]  # Radii

# Initialize the results dictionary
results = {}

# Track all previously calculated (xc, yc, r) combinations
previous_coords = {}

# Calculate xc and yc for each combination
for xi, yi in coordinates:
    coord_key = f"({xi},{yi})"
    results[coord_key] = {}
    
    for ri in r_values:
        results[coord_key][f"r={ri}"] = {}
        
        for theta_i in [-90, 0, 90, 180]:  # degrees for output
            theta_rad = np.radians(theta_i)
            xc = round(xi - ri * np.cos(theta_rad))
            yc = round(yi - ri * np.sin(theta_rad))
            vote_key = (xc, yc, ri)  # Include r in the key for unique combinations
            
            # Initialize vote count if not already present
            if vote_key not in previous_coords:
                previous_coords[vote_key] = 0
            
            # Increment the vote count
            previous_coords[vote_key] += 1
            
            results[coord_key][f"r={ri}"][theta_i] = {
                "xc": xc,
                "yc": yc,
                "vote": previous_coords[vote_key]
            }

# Save results to JSON
with open('coordinates_reorganized.json', 'w') as json_file:
    json.dump(results, json_file, indent=4)

print("Calculations completed and saved to coordinates_reorganized.json")
