from utils_NXP_N7 import *  # noqa: F403,F401
from model_definition import create_model_CNN
import tensorflow as tf
import numpy as np
from sklearn.metrics import precision_score, recall_score, f1_score

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


# # Matrice de confusion
# y_pred = np.argmax(model.predict(X_test), axis=-1)
# cm = tf.math.confusion_matrix(y_test, y_pred, num_classes = 10)

# cm = cm.numpy()

# Prédictions + matrice de confusion
y_pred_logits = model.predict(X_test)
y_pred = np.argmax(y_pred_logits, axis=1)

cm = tf.math.confusion_matrix(y_test, y_pred)
class_names = [str(c) for c in np.unique(y_train)]
cm = cm.numpy() 
print("\nMatrice de confusion :\n", cm)
plot_confusion_matrix(cm, classes=class_names, title="Confusion Matrix")


# Calcul des métriques : précision, rappel, F1-score
precision = precision_score(y_test, y_pred, average='weighted')
recall    = recall_score(y_test, y_pred, average='weighted')
f1        = f1_score(y_test, y_pred, average='weighted')

print(f"\nPrecision : {precision}")
print(f"Recall    : {recall}")
print(f"F1-score  : {f1}")


# Suppresion de la derniere couche pour compatibilité NXP
model_trunc = tf.keras.Model(inputs=model.inputs, outputs=model.layers[-2].output)
# Sauvegarde du modèle
model_trunc.save("model_saved")
print("model_trunc saved.")

plot_scores(history)  # Plot validation accuracy
