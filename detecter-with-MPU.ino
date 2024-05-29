#include <Wire.h>
#include "MPU6050.h"
#include "MicroFlow.h"

MPU6050 mpu;

// Architecture du réseau de neurones
int topology[] = {3, 2, 3}; // 3 entrées, 2 neurones cachés, 3 sorties
int layers = 3; // Nombre total de couches dans le réseau

// Poids obtenus par l'entraînement
double weights[] = {-0.8215193, -0.3065338, -0.2264787, -0.7059456, -0.77381235, -0.6342252, -1.0969148, 0.16654213, -0.29844505, -0.62380487, -0.628445, -0.28441197};

// Biais obtenus par l'entraînement
double biases[] = {0.3044409, 0.39704528, -0.3200404, -0.33367547, -0.34890598};

// Initialisation du réseau de neurones
MicroMLP mlp(layers, topology, weights, biases, TANH);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  // Vérifiez si la connexion au MPU6050 a été établie
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  // Variables pour stocker les valeurs des capteurs
  int16_t ax, ay, az;
  
  // Lire les valeurs des capteurs
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Normalisation des valeurs (ajustez cette partie en fonction de votre prétraitement)
  double inputs[] = {ax / 16384.0, ay / 16384.0, az / 16384.0}; // Normaliser les valeurs
  double output[1] = {};
  
  // Passer les valeurs d'entrée à travers le réseau de neurones
  mlp.feedforward(inputs, output);
  
  // Afficher les résultats sur le moniteur série
  Serial.print("Inputs: "); Serial.print(inputs[0]); Serial.print(", "); Serial.print(inputs[1]); Serial.print(", "); Serial.println(inputs[2]);
  Serial.print("Neural Network Output: "); Serial.println(output[0]);
  
  // Déterminer si le geste est désiré ou non (ajustez ce seuil en fonction de votre modèle)
  double threshold = 0.5; // Remplacez ce seuil par une valeur appropriée basée sur vos sorties
  if (output[0] > threshold) {
    Serial.println("X");
  } else {
    Serial.println("Geste non détecté");
  }
  Serial.println("---------------------");
  delay(500); // Attendre un peu avant de refaire une lecture
}




