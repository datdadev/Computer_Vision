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
theta = np.radians([-180, -90, 0, 90, 180])  # Convert to radians
r = [2, 3]

# Initialize the results dictionary
results = {}

# Track all previously calculated xc, yc combinations
previous_coords = {}

# Calculate xc and yc for each combination
for xi, yi in coordinates:
    coord_key = f"({xi},{yi})"
    results[coord_key] = {}
    
    for theta_i in [-180, -90, 0, 90, 180]:  # degrees for output
        theta_rad = np.radians(theta_i)
        results[coord_key][theta_i] = {}
        
        for ri in r:
            xc = round(xi - ri * np.cos(theta_rad))
            yc = round(yi - ri * np.sin(theta_rad))
            vote_key = ((xc, 6), (yc, 6))  # Use rounded values to avoid floating-point issues
            
            # Initialize vote if not already present
            if vote_key not in previous_coords:
                previous_coords[vote_key] = 0
            
            # Increment the vote count
            previous_coords[vote_key] += 1
            
            results[coord_key][theta_i][f"r={ri}"] = {
                "xc": xc,
                "yc": yc,
                "vote": previous_coords[vote_key]
            }

# Save results to JSON
with open('coordinates.json', 'w') as json_file:
    json.dump(results, json_file, indent=4)

print("Calculations completed and saved to coordinates.json")
