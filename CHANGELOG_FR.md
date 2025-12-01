## [1.2.2] - 30-11-2025
### Ajouts
### Modifications

# [1.3.0] - 2025-12-01
### Modifications
- Réécriture complète de `src/main.cpp` pour fiabiliser la connexion WiFi, afficher l'IP périodiquement dans le Moniteur Série et fournir un tableau de bord web fonctionnel.
### Ajouts
- Interface web simple et robuste avec les routes `/`, `/data`, `/env`, `/action`.
- Bascule automatique en AP de secours (`UTB_AP` / `utb12345`) si la STA échoue.

## [1.3.1] - 01-12-2025
### Corrections
- L’UI Web renvoie du JSON valide (utilise `null` au lieu de `NaN`), le moniteur réseau affiche désormais correctement les données.
- Ajout de l’encodage UTF-8 (meta et en-têtes HTTP) pour l’affichage des accents dans l’interface.

## [1.3.2] - 01-12-2025
### Ajouts
- Le Moniteur Série affiche désormais le statut WiFi détaillé (SSID, IP, RSSI, DNS, mode) à la connexion et périodiquement.
- Détection de la PSRAM ajoutée aux logs de démarrage (présence et taille), ainsi que la taille/vitesse de la Flash.

## [1.3.3] - 01-12-2025
### Modifications
- Politique des logs série: désormais silencieux en fonctionnement normal; impression seulement lors des événements de (re)connexion et de déconnexion. Évite les répétitions d’IP.
# Journal des Modifications (Changelog)

Tous les changements notables du projet "Universal Test Bench" seront documentés ici.

## [1.2.0] - 30-11-2025
### Ajouts
- Affichage des informations réseau dans le Moniteur Série (IP, Passerelle, DNS, SSID, RSSI).
## [1.2.1] - 30-11-2025
### Corrections
- Connexion WiFi plus robuste et journaux du Moniteur Série plus explicites (IP et URL Web à la connexion ; rappel périodique de l’IP).
- Documentation mise à jour : suppression des références au kit, guides capteurs rendus universels.


### Modifications
- Suppression des dépendances inutilisées (UniversalTelegramBot, ArduinoJson) pour éliminer les avertissements de dépréciation.
- Code optimisé pour les dernières versions des bibliothèques.

## [1.1.0] - 26-11-2023
### Ajouts
- **Matériel :**
    - Support des capteurs DHT11/DHT22 (Temp/Humidité) sur le GPIO 16.
    - Support du Buzzer (Actif/Passif) sur le GPIO 17.
    - Support du NeoPixel (LED RGB) intégré à l'ESP32-S3 sur le GPIO 48.
- **Interface :**
    - Nouvelle section sur l'écran TFT pour les données environnementales.
    - Ajout de boutons d'action sur l'interface Web pour tester le Buzzer et la LED.
- **Système :**
    - Le Nom du Projet et la Version sont maintenant définis dans `platformio.ini`.

## [1.0.0] - 26-11-2023
### Ajouts
- Version initiale.
- Test universel Analogique/Numérique.
- Intégration Serveur Web & Écran TFT.