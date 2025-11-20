from utils_NXP_N7 import *  # noqa: F403,F401
from model_definition import create_model_CNN
import tensorflow as tf

# Charger les données
X_train, X_test, y_train, y_test = load_data(
    "data_processed/data_processeddata_df.csv",
    "data_processed/data_processedlabels_df.csv"
)

# Définir les paramètres du modèle
input_shape = X_train.shape[1:]  # Forme des données d'entrée
num_classes = len(np.unique(y_train))  # Nombre de classes pour la classification
model = create_model_CNN(input_shape, num_classes)

# Compiler le modèle
model.compile(
    optimizer='adam',
    loss='sparse_categorical_crossentropy',
    metrics=['accuracy']
)

# Définir les callbacks
callbacks = [
    tf.keras.callbacks.EarlyStopping(
        monitor='val_loss',
        patience=10,            
        restore_best_weights=True
    ),
    tf.keras.callbacks.ReduceLROnPlateau(
        monitor='val_loss',
        factor=0.5,
        patience=10,
        min_lr=1e-6
    )
]

# Entraîner le modèle
history = model.fit(
    X_train, y_train,
    epochs=3000,
    batch_size=16,
    validation_split=0.2,
    callbacks=callbacks
)

# Évaluer le modèle
test_loss, test_accuracy = model.evaluate(X_test, y_test)
print(f'Test Loss: {test_loss}, Test Accuracy: {test_accuracy}')

plot_scores(history)  # Plot validation accuracy
   
