import serial 
import csv
import re
import os
import pandas as pd
import numpy as np

# User settings
dataset_folder_path = "C:/Users/bilaaaaaaal/Documents/ENSEEIHT/S8/projet_smart_sensor/frdmmcxn9474_22_08_2024_fixed/data"  # Path to the dataset folder
char = '9'  # The character or label for the data being recorded
BaudRate = 115200 #460800  # Baud rate for the serial communication
COM_port = 'COM6' # COM port used for the serial communication

# Function to generate a new unique filename if the base file already exists
def generate_new_filename(base_filename):
    if not os.path.exists(base_filename):  # If the file doesn't exist, use the base filename
        return base_filename
    base, extension = os.path.splitext(base_filename)  # Split the base filename and its extension
    i = 1
    new_filename = f"{base}_{i}{extension}"  # Generate a new filename by appending a number
    while os.path.exists(new_filename):  # If the new filename exists, increment the number
        i += 1
        new_filename = f"{base}_{i}{extension}"
    return new_filename  # Return the unique filename

# Initialize the serial port for communication
ser = serial.Serial(COM_port, BaudRate)  # Set the COM port and baud rate

# Regular expression pattern to parse the incoming serial data (expects data in "x: value y: value" format)
pattern = re.compile(r"x:\s*(-?\d+\.?\d*)\s*y:\s*(-?\d+\.?\d*)")

while True:
    print("Recording")
    print(char)
    print("...")
    # Set up folder and filename for saving the CSV
    base_csv_filename = char + '.csv'
    base_folder_name = char
    # Create the folder if it doesn't exist
    if not os.path.isdir(dataset_folder_path + "/" + base_folder_name):
        os.mkdir(dataset_folder_path + "/" + base_folder_name)
    # Create the full path for the CSV file
    base_csv_filename = dataset_folder_path + "/" + base_folder_name + "/" + base_csv_filename
    csv_path = generate_new_filename(base_csv_filename)  # Ensure a unique filename
    # Open the CSV file for writing
    csv_file = open(csv_path, mode='w', newline='')
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(['X', 'Y'])  # Write the header row for X and Y data
    # Start reading data from the serial port
    print("Starting...\r\n")
    #for i in range(3285):
    while True:
        line = ser.readline().decode('utf-8').strip()  # Read a line from the serial port and decode it
        if line == 'End':  # Stop recording when 'End' is received
            break
        # Match the line with the expected "x: value y: value" format
        match = pattern.match(line)
        if match:
            x, y = match.groups()  # Extract the X and Y values
            csv_writer.writerow([x, y])  # Write the X and Y values to the CSV file
    # Close the CSV file after data recording is finished
    csv_file.close()
    print("End\r\n")

# Close the serial port when finished
ser.close()
