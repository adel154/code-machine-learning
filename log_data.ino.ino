//log_data.ino
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define THRESHOLD 20 // Seuil pour la détection de mouvement
#define READINGS_PER_SAMPLE 40 // Nombre d'échantillons à lire par détection

Adafruit_MPU6050 mpu; // Création d'un objet MPU6050

int count =1; // Compteur pour les séquences de données
float x_initial, y_initial, z_initial; // Valeurs initiales pour la calibration

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Attente de l'initialisation du port série

  Serial.println("Adafruit MPU6050 test!");

  // Attente de l'initialisation du port série
  if (!mpu.begin(0x69)) {
    Serial.println("Failed to find MPU6050 chip"); 
    while (1) {
      delay(10); // Boucle infinie si le capteur n'est pas trouvé
    }
  }
    Serial.println("MPU6050 Found!");
	
// Configuration des plages et de la bande passante du capteur
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100); // Délai pour stabiliser le capteur
  calibrate_mpu(); // Calibration du capteur
  
}

void loop() {
  detect_motion(); // Détection de mouvement dans la boucle principale
  
}

// Fonction pour lire et afficher les données d'accélération
void read_data(){
  
  for(int i =0;i<40;i++){
  sensors_event_t a, g, temp; // Création des événements de capteur
  mpu.getEvent(&a, &g, &temp); // Récupération des données du capteur
 
  Serial.print(a.acceleration.x );
  Serial.print(",");
  Serial.print(a.acceleration.y );
  Serial.print(",");
  Serial.println(a.acceleration.z);
  delay(10); // Délai pour synchroniser les lectures
  }
  // Affichage de la séparation entre les séquences de données
  Serial.println("");
  Serial.println("--------");
  Serial.println(count);
  Serial.println("--------");
  count++;
}
// Fonction de calibration du capteur
void calibrate_mpu(){
  float totX, totY, totZ;
  sensors_event_t a, g, temp;
  
  // Lecture de 10 échantillons pour la calibration
  for (int i = 0; i < 10; i++) {
    mpu.getEvent(&a, &g, &temp);
    totX = totX + a.acceleration.x;
    totY = totY + a.acceleration.y;
    totZ = totZ + a.acceleration.z;
  }
  // Calcul des moyennes pour les valeurs initiales
  x_initial = totX / 10;
  y_initial = totY / 10;
  z_initial = totZ / 10;
  Serial.println("Calibrated");
}

void detect_motion(){
   sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  if( abs(a.acceleration.x - x_initial) +abs(a.acceleration.y - y_initial) + abs(a.acceleration.z - z_initial) > 20){
    read_data(); // Enregistrement des données si le mouvement est détecté
  }
  else{
    delay(5); // Délai pour éviter une boucle trop rapide
  }
}
