# Guide Rapide des Capteurs (v1.3.1)

## 🔴 Broches d'Entrée Universelles

### 1. Sur le GPIO 13 (Entrée Numérique)
*Branchez ici les capteurs qui donnent un état "Tout ou Rien" (0 ou 1).*
* Boutons, Tilt (Bille/Mercure), Switch Reed, Capteur de Choc.
* Obstacle Infrarouge, Fourche Optique, PIR (Mouvement).
* Capteurs de Son (Microphone sortie D0).

### 2. Sur le GPIO 14 (Entrée Analogique)
*Branchez ici les capteurs qui donnent une valeur variable.*
* Joystick (Axe X ou Y), Potentiomètres.
* LDR (Lumière), Niveau d'eau, Humidité du sol.
* Thermistance, Effet Hall Analogique.

---

## 🟢 Modules Spéciaux

Pour les modules courants, utilisez les broches dédiées suivantes :

### 1. Température & Humidité (DHT11)
* **Branchement :** Signal sur **GPIO 16**.
* **Utilisation :** Les valeurs s'affichent en bas de l'écran TFT et dans la section "Env" de l'interface Web (accents UTF-8 pris en charge).

### 2. Buzzer (Actif ou Passif)
* **Branchement :** Signal (I/O) sur **GPIO 17**.
* **Utilisation :** Appuyez sur le bouton **"BUZZER"** de l'interface Web pour déclencher une mélodie de test.

### 3. LED RGB (NeoPixel)
* **Branchement :** Aucun ! Utilise la **LED intégrée (GPIO 48)** de la carte.
* **Utilisation :** Appuyez sur le bouton **"RGB LED"** de l'interface Web pour lancer un cycle de couleurs.