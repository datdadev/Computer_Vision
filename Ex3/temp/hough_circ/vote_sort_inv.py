import json

# Load JSON data from coordinates_reorganized.json
with open('coordinates_reorganized.json', 'r') as file:
    data = json.load(file)

# Dictionary to store the highest votes for each unique [xc, yc] pair
highest_votes_dict = {}

# Loop through each coordinate entry in the JSON data
for key, radii in data.items():
    for radius, angles in radii.items():
        for angle, values in angles.items():
            # Extract xc, yc, and vote
            xc = values['xc']
            yc = values['yc']
            vote = values['vote']
            coord_key = (xc, yc)

            # Only include votes >= 3 and ensure the highest vote is kept
            if vote >= 3:
                # Check if this (xc, yc) has been seen or if the current vote is higher
                if coord_key not in highest_votes_dict or vote > highest_votes_dict[coord_key][0]:
                    highest_votes_dict[coord_key] = (vote, [xc, yc])

# Convert the result into the required format
votes_dict = {}
for vote, coords in highest_votes_dict.values():
    if vote not in votes_dict:
        votes_dict[vote] = []
    votes_dict[vote].append(coords)

# Print or save the output to verify
print(votes_dict)

# Optionally, save the result to a new JSON file
with open('votes_coordinates_filtered.json', 'w') as output_file:
    json.dump(votes_dict, output_file, indent=4)

print("Filtered votes and coordinates saved to votes_coordinates_filtered.json")
