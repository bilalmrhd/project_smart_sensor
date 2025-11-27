import tensorflow as tf
from tensorflow.keras import layers, Sequential

def create_model_CNN(input_shape, num_classes):

    model = Sequential(name = "CNN_Model")

    # Input Layer
    model.add(layers.InputLayer(input_shape=input_shape))

    #Bloc 1 
    model.add(layers.Conv1D(
        filters=32,
        kernel_size=3,
        padding='same',
        activation='relu',
        input_shape=(3286,2)
    ))
    model.add(layers.BatchNormalization())

    model.add(layers.GlobalAveragePooling1D(pool_size=2))

    # Dropout 
    model.add(layers.Dropout(0.5))

    #Bloc 2
    model.add(layers.Conv1D(
        filters=32,
        kernel_size=5,
        padding='same',
        activation='relu',
    ))
    model.add(layers.BatchNormalization())

    # #Bloc 3
    # model.add(layers.Conv1D(
    #     filters=128,
    #     kernel_size=3,
    #     padding='same',
    #     activation='relu',  
    # ))
    # model.add(layers.BatchNormalization())


    # Dropout 
    model.add(layers.Dropout(0.5))

    model.add(layers.GlobalAveragePooling1D())



    model.add(layers.Flatten())

    model.add(layers.Dense(num_classes, activation='relu'))

    # Couche de sortie : LOGITS, sans softmax
    #model.add(layers.Dense(num_classes, activation=None, name="logits"))

    # Output Layer
    model.add(layers.Dense(num_classes, activation='softmax')) #softmax pas supporté par la carte
    


    return model
    

print("Model definition complete at 100%.")