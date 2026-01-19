import pandas as pd
import os
import numpy as np

# User settings
dataset_path = "C://Users//bilaaaaaaal//Documents//ENSEEIHT//S8//projet_smart_sensor//data_bilal"  # Path to the folder containing the CSV files
X_Y_folder_path = "C://Users//bilaaaaaaal//Documents//ENSEEIHT//S8//projet_smart_sensor//frdmmcxn9474_22_08_2024_fixed//data_processed_1"  # Path to the folder where the processed files will be saved

data = pd.DataFrame()  # Initialize an empty DataFrame to store the concatenated data
i = 0  # Counter to track the series ID for each CSV file

# Walk through the dataset folder, looking for CSV files
for subdir, dirs, files in os.walk(dataset_path):  # os.walk generates file names in the dataset directory
    for file in files:
        if file.endswith('.csv'):  # Check if the file is a CSV
            file_path = os.path.join(subdir, file)  # Get the full path to the file
            df = pd.read_csv(file_path)  # Read the CSV file into a DataFrame
            capture = np.array(df)  # Convert the DataFrame into a NumPy array for easier manipulation
            label = os.path.basename(subdir)  # Extract the label from the subfolder's name (assumed to be the class)

            # Convert the array back into a DataFrame with columns 'X' and 'Y'
            temp = pd.DataFrame(capture, columns=['X', 'Y'])  # Create a DataFrame with the 'X' and 'Y' columns
            temp['point'] = range(len(temp))  # Add a 'point' column to track the index of each point in the sequence
            temp['series_id'] = i  # Add a 'series_id' column to uniquely identify each series (incremental ID)
            temp['label'] = label  # Add a 'label' column to identify the class label (based on the folder name)
            i += 1  # Increment the series ID for the next CSV file

            # Append the current DataFrame (temp) to the overall data DataFrame
            data = pd.concat([data, temp])

# Reset the index of the final DataFrame to ensure a clean, continuous index
data.reset_index(drop=True, inplace=True)

# Save the 'X', 'Y', 'point', and 'series_id' columns to a CSV file in the output folder
pd.concat((data['X'], data['Y'], data['point'], data['series_id']), axis=1).to_csv(X_Y_folder_path + "data_df.csv")

# Save the 'series_id' and 'label' columns to a separate CSV file for labels
pd.concat((data['series_id'], data['label']), axis=1).to_csv(X_Y_folder_path + "labels_df.csv")

print("Data processing complete. Processed files saved.")