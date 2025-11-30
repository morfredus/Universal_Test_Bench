# Universal Test Bench (Banc d'Essai Universel) 🛠️

**Version :** 1.3.2  
**Carte :** ESP32-S3 DevKitC-1 (N16R8)  
**Framework :** Arduino (via PlatformIO)

## 📖 Présentation
Le **Universal Test Bench** est un outil de diagnostic pour tester rapidement des capteurs courants (numériques/analogiques) ainsi que des modules spécifiques (DHT, Buzzer) sans reprogrammer la carte. Il affiche les données en temps réel sur un écran TFT et via une interface Web.

## ✨ Fonctionnalités (v1.3.1)
* **Diagnostics WiFi :** MAC, événements, affichage périodique de l’IP.
* **AP de secours :** Création de `UTB_AP` si la connexion standard échoue.
* **mDNS :** Accès via `http://utb.local` (si pris en charge).
* **Test Universel :** Broches dédiées pour capteurs Analogiques et Numériques.
* **Support Spécial :** Prise en charge native du DHT11/22, du Buzzer et du NeoPixel intégré.
* **Affichage Temps Réel :**
    * **Écran TFT :** Affiche les valeurs, états logiques, température et humidité.
    * **Interface Web :** Tableau de bord avec boutons d'action; accents UTF-8 pris en charge; le moniteur réseau affiche correctement les données.
    * **Moniteur Série :** Informations WiFi détaillées au démarrage et périodiquement (IP, SSID, RSSI, DNS, mode). Détection PSRAM affichée.
* **Configuration Centralisée :** Nom et Version gérés dans `platformio.ini`.

## 🔌 Câblage et Pinout

### 1. Entrées Universelles
| Type | GPIO Pin | Capteurs Supportés |
| :--- | :--- | :--- |
| **ENTRÉE NUMÉRIQUE** | **GPIO 13** | Bouton, Tilt, Switch Reed, Obstacle IR, Son... |
| **ENTRÉE ANALOGIQUE** | **GPIO 14** | Potentiomètre, Joystick, LDR, Eau, Flamme... |

### 2. Modules Spéciaux (Ajout v1.1.0)
| Module | GPIO Pin | Notes |
| :--- | :--- | :--- |
| **DHT11 / DHT22** | **GPIO 16** | Capteur Température & Humidité |
| **Buzzer** | **GPIO 17** | Buzzer Actif ou Passif |
| **NeoPixel (RGB)** | **GPIO 48** | **LED Intégrée** (Sur la carte DevKitC-1) |

## 🚀 Installation
1.  Ouvrez le projet avec **PlatformIO**.
2.  Créez le fichier `include/secrets.h` avec vos identifiants WiFi.
3.  Téléversez le code.
4.  L'adresse IP s'affiche sur l'écran TFT et le moniteur série.

## ⚙️ Configuration
* **WiFi :** `include/secrets.h`
* **Version du projet :** Modifiable dans `platformio.ini` (build_flags).