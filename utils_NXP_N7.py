import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import itertools
from tensorflow.keras.preprocessing.sequence import pad_sequences

# Function to load data and prepare it for training/testing
def load_data(test_data_path, test_labels_path):
    # Load the feature data (X) and label data (y) from CSV files
    df_X = pd.read_csv(test_data_path)  # Read the features dataset
    df_X = df_X.drop(columns=['Unnamed: 0'])  # Drop the unnamed index column
    df_y = pd.read_csv(test_labels_path)  # Read the labels dataset
    df_y = df_y.drop(columns=['Unnamed: 0'])  # Drop the unnamed index column

    # Determine the maximum length of sequences by taking the first series
    max_length = len(df_X[df_X.series_id == 1])

    # Remove duplicate entries in the labels dataset based on the 'series_id' column
    df_y = df_y.drop_duplicates(subset=['series_id'], ignore_index=True)

    # Initialize lists to store data and labels
    data = []
    labels = []
    # Loop through each unique series in the feature dataset
    for series_id in df_X.series_id.unique():
        # Extract the feature values for each series, dropping unnecessary columns
        temp = df_X[df_X.series_id == series_id].drop(columns=['series_id', 'point'], axis=1).values
        data.append(temp)  # Append the features to the data list
    # Get the corresponding labels
    labels = df_y.label.values

    # Compute max length for padding
    max_len = max(len(seq) for seq in data)

    # Create a dictionary to store the data and labels
    data = {'data': data, 'label': labels}

    # Encode the labels as integers using LabelEncoder
    le = LabelEncoder()
    data['label'] = le.fit_transform(data['label'])  # Transform labels into numeric values

    # Split the dataset into training and test sets
    X_train, X_test, y_train, y_test = train_test_split(data['data'], data['label'], test_size=0.2, random_state=42997)
    # Pad sequences to ensure uniform length
    X_train = pad_sequences(X_train, maxlen=max_len, dtype='int16')
    X_test = pad_sequences(X_test, maxlen=max_len, dtype='int16')
    y_train = np.array(y_train, dtype=np.int8)
    y_test = np.array(y_test, dtype=np.int8)

    # Shuffle the training data
    idx = np.random.permutation(len(X_train))  # Generate a random permutation of indices
    X_train = X_train[idx]  # Shuffle the features
    y_train = y_train[idx]  # Shuffle the labels
    return X_train, X_test, y_train, y_test  # Return the split and shuffled data


# Function to plot accuracy and loss curves from model training history
def plot_scores(history, metric="accuracy", loss="loss"):
    metric = "accuracy"  # Set the metric to "accuracy"
    plt.figure(figsize=(20,10))  # Create a new figure with specified size
    # Plot accuracy
    plt.subplot(1,2,1)  # Create the first subplot (left side)
    plt.plot(history.history[metric])  # Plot training accuracy
    plt.plot(history.history["val_" + metric])  # Plot validation accuracy
    plt.title("model " + metric)  # Set the title of the plot
    plt.ylabel(metric, fontsize="large")  # Set the y-axis label
    plt.xlabel("epoch", fontsize="large")  # Set the x-axis label
    plt.legend(["train", "val"], loc="best")  # Add a legend
    plt.grid()  # Show grid lines

    # Plot loss
    plt.subplot(1,2,2)  # Create the second subplot (right side)
    plt.plot(history.history[loss])  # Plot training loss
    plt.plot(history.history["val_" + loss])  # Plot validation loss
    plt.title("model " + loss)  # Set the title of the plot
    plt.ylabel(loss, fontsize="large")  # Set the y-axis label
    plt.xlabel("epoch", fontsize="large")  # Set the x-axis label
    plt.legend(["train", "val"], loc="best")  # Add a legend
    plt.grid()  # Show grid lines
    # Display the plots
    plt.show()
    plt.close()  # Close the plot to free memory


# Function to plot a confusion matrix
def plot_confusion_matrix(cm, classes, normalize=False, title='Confusion matrix', cmap=plt.cm.Blues):
    plt.imshow(cm, interpolation='nearest', cmap=cmap)  # Display the confusion matrix as an image
    plt.title(title)  # Set the title of the plot
    plt.colorbar()  # Add a color bar to the right of the matrix
    tick_marks = np.arange(len(classes))  # Create tick marks based on the number of classes
    plt.xticks(tick_marks, classes, rotation=45)  # Set the class names on the x-axis
    plt.yticks(tick_marks, classes)  # Set the class names on the y-axis

    # Loop through the matrix and annotate each cell with its value
    thresh = cm.max() / 2.  # Set the threshold for text color based on the maximum value in the matrix
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j], horizontalalignment="center", color="white" if cm[i, j] > thresh else "black")

    # Adjust layout to fit everything
    plt.tight_layout()
    plt.ylabel('True label')  # Set the y-axis label
    plt.xlabel('Predicted label')  # Set the x-axis label

# import numpy as np
# import pandas as pd
# from sklearn.preprocessing import LabelEncoder
# from sklearn.model_selection import train_test_split
# import matplotlib.pyplot as plt
# import itertools

# # Function to load data and prepare it for training/testing
# def load_data(test_data_path, test_labels_path):
#     # Load the feature data (X) and label data (y) from CSV files
#     df_X = pd.read_csv(test_data_path)  # Read the features dataset
#     df_X = df_X.drop(columns=['Unnamed: 0'])  # Drop the unnamed index column
#     df_y = pd.read_csv(test_labels_path)  # Read the labels dataset
#     df_y = df_y.drop(columns=['Unnamed: 0'])  # Drop the unnamed index column

#     # Determine the maximum length of sequences by taking the first series
#     max_length = len(df_X[df_X.series_id == 1])

#     # Remove duplicate entries in the labels dataset based on the 'series_id' column
#     df_y = df_y.drop_duplicates(subset=['series_id'], ignore_index=True)

#     # Initialize lists to store data and labels
#     data = []
#     labels = []
#     # Loop through each unique series in the feature dataset
#     for series_id in df_X.series_id.unique():
#         # Extract the feature values for each series, dropping unnecessary columns
#         temp = df_X[df_X.series_id == series_id].drop(columns=['series_id', 'point'], axis=1).values
#         data.append(temp)  # Append the features to the data list
#     # Get the corresponding labels
#     labels = df_y.label.values
#     # Create a dictionary to store the data and labels
#     data = {'data': data, 'label': labels}

#     # Encode the labels as integers using LabelEncoder
#     le = LabelEncoder()
#     data['label'] = le.fit_transform(data['label'])  # Transform labels into numeric values

#     # Split the dataset into training and test sets
#     X_train, X_test, y_train, y_test = train_test_split(data['data'], data['label'], test_size=0.2, random_state=42997)
#     # Convert the training and test data to NumPy arrays for compatibility with machine learning algorithms
#     X_train = np.array(X_train, dtype=np.int16)
#     X_test = np.array(X_test, dtype=np.int16)
#     y_train = np.array(y_train, dtype=np.int8)
#     y_test = np.array(y_test, dtype=np.int8)

#     # Shuffle the training data
#     idx = np.random.permutation(len(X_train))  # Generate a random permutation of indices
#     X_train = X_train[idx]  # Shuffle the features
#     y_train = y_train[idx]  # Shuffle the labels
#     return X_train, X_test, y_train, y_test  # Return the split and shuffled data


# # Function to plot accuracy and loss curves from model training history
# def plot_scores(history, metric="accuracy", loss="loss"):
#     metric = "accuracy"  # Set the metric to "accuracy"
#     plt.figure(figsize=(20,10))  # Create a new figure with specified size
#     # Plot accuracy
#     plt.subplot(1,2,1)  # Create the first subplot (left side)
#     plt.plot(history.history[metric])  # Plot training accuracy
#     plt.plot(history.history["val_" + metric])  # Plot validation accuracy
#     plt.title("model " + metric)  # Set the title of the plot
#     plt.ylabel(metric, fontsize="large")  # Set the y-axis label
#     plt.xlabel("epoch", fontsize="large")  # Set the x-axis label
#     plt.legend(["train", "val"], loc="best")  # Add a legend
#     plt.grid()  # Show grid lines

#     # Plot loss
#     plt.subplot(1,2,2)  # Create the second subplot (right side)
#     plt.plot(history.history[loss])  # Plot training loss
#     plt.plot(history.history["val_" + loss])  # Plot validation loss
#     plt.title("model " + loss)  # Set the title of the plot
#     plt.ylabel(loss, fontsize="large")  # Set the y-axis label
#     plt.xlabel("epoch", fontsize="large")  # Set the x-axis label
#     plt.legend(["train", "val"], loc="best")  # Add a legend
#     plt.grid()  # Show grid lines
#     # Display the plots
#     plt.show()
#     plt.close()  # Close the plot to free memory


# # Function to plot a confusion matrix
# def plot_confusion_matrix(cm, classes, normalize=False, title='Confusion matrix', cmap=plt.cm.Blues):
#     plt.imshow(cm, interpolation='nearest', cmap=cmap)  # Display the confusion matrix as an image
#     plt.title(title)  # Set the title of the plot
#     plt.colorbar()  # Add a color bar to the right of the matrix
#     tick_marks = np.arange(len(classes))  # Create tick marks based on the number of classes
#     plt.xticks(tick_marks, classes, rotation=45)  # Set the class names on the x-axis
#     plt.yticks(tick_marks, classes)  # Set the class names on the y-axis

#     # Loop through the matrix and annotate each cell with its value
#     thresh = cm.max() / 2.  # Set the threshold for text color based on the maximum value in the matrix
#     for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
#         plt.text(j, i, cm[i, j], horizontalalignment="center", color="white" if cm[i, j] > thresh else "black")

#     # Adjust layout to fit everything
#     plt.tight_layout()
#     plt.ylabel('True label')  # Set the y-axis label
#     plt.xlabel('Predicted label')  # Set the x-axis label