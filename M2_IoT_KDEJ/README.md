# Guide Rapide - ESP32 MQTT avec Docker

## 📋 Prérequis
- Docker Desktop installé et lancé
- ESP32 avec DHT22
- PlatformIO (VS Code)

---

## 🚀 Installation Rapide

### 1. Créer la structure de dossiers
```powershell
mkdir C:\mosquitto\config, C:\mosquitto\data, C:\mosquitto\log
```

### 2. Créer le fichier de configuration
```powershell
notepad C:\mosquitto\config\mosquitto.conf
```

Copier ce contenu :
```conf
listener 1883 0.0.0.0
allow_anonymous true
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log
log_dest stdout
```

### 3. Lancer le broker MQTT
```powershell
docker run -d `
  --name mosquitto `
  -p 1883:1883 `
  -v C:\mosquitto\config\mosquitto.conf:/mosquitto/config/mosquitto.conf `
  -v C:\mosquitto\data:/mosquitto/data `
  -v C:\mosquitto\log:/mosquitto/log `
  --restart unless-stopped `
  eclipse-mosquitto
```

### 4. Trouver l'IP de votre PC
```powershell
ipconfig
```
Chercher l'IPv4 de votre adaptateur WiFi/Ethernet (ex: `192.168.1.100`)

### 5. Installer les outils MQTT (optionnel mais recommandé)
```powershell
choco install mosquitto
```

---

## 🔧 Configuration ESP32

### Dans `config.h` :
```cpp
const char* MQTT_BROKER = "192.168.1.100";  // ⚠️ IP DE VOTRE PC
const int MQTT_PORT = 1883;
```

### Dans `platformio.ini` :
```ini
lib_deps =
    adafruit/DHT sensor library @ ^1.4.6
    knolleary/PubSubClient @ ^2.8
    bblanchon/ArduinoJson @ ^7.2.1
```

---

## 📡 Commandes Utiles

### Écouter tous les messages
```powershell
mosquitto_sub -h localhost -t "home/climate/#" -v
```

### Écouter un topic spécifique
```powershell
mosquitto_sub -h localhost -t "home/climate/sensors" -v
```

### Envoyer une commande LED ON
```powershell
mosquitto_pub -h localhost -t "home/climate/command" -m "LED_ON"
```

### Envoyer une commande LED OFF
```powershell
mosquitto_pub -h localhost -t "home/climate/command" -m "LED_OFF"
```

### Tester la connexion
```powershell
# Terminal 1 - Subscribe
mosquitto_sub -h localhost -t "test/topic" -v

# Terminal 2 - Publish
mosquitto_pub -h localhost -t "test/topic" -m "Hello MQTT"
```

---

## 🐳 Gestion Docker

### Vérifier le statut
```powershell
docker ps
```

### Voir les logs
```powershell
docker logs mosquitto
docker logs -f mosquitto  # Suivre en temps réel
```

### Redémarrer le broker
```powershell
docker restart mosquitto
```

### Arrêter le broker
```powershell
docker stop mosquitto
```

### Démarrer le broker
```powershell
docker start mosquitto
```

### Supprimer le container
```powershell
docker stop mosquitto
docker rm mosquitto
```

### Recréer le container (si besoin)
```powershell
docker run -d `
  --name mosquitto `
  -p 1883:1883 `
  -v C:\mosquitto\config\mosquitto.conf:/mosquitto/config/mosquitto.conf `
  -v C:\mosquitto\data:/mosquitto/data `
  -v C:\mosquitto\log:/mosquitto/log `
  --restart unless-stopped `
  eclipse-mosquitto
```

---

## 📊 Topics MQTT Utilisés

| Topic | Description | Type |
|-------|-------------|------|
| `home/climate/sensors` | Données JSON complètes | Publish |
| `home/climate/temperature` | Température seule | Publish |
| `home/climate/humidity` | Humidité seule | Publish |
| `home/climate/heatindex` | Index de chaleur | Publish |
| `home/climate/status` | Statut online/offline | Publish |
| `home/climate/command` | Commandes (LED_ON/OFF) | Subscribe |

---

## 🔍 Dépannage

### L'ESP32 ne se connecte pas ?
1. Vérifier l'IP dans `config.h`
   ```powershell
   ipconfig
   ```
2. Vérifier que le broker écoute sur `0.0.0.0`
   ```powershell
   docker logs mosquitto | Select-String "listen"
   ```
3. Vérifier que le port 1883 est ouvert
   ```powershell
   netstat -an | Select-String "1883"
   ```

### Le broker ne démarre pas ?
```powershell
docker logs mosquitto
```

### Réinitialiser complètement
```powershell
docker stop mosquitto
docker rm mosquitto
Remove-Item -Recurse -Force C:\mosquitto\data\*
Remove-Item -Recurse -Force C:\mosquitto\log\*
# Puis relancer le container
```

---

## ⚡ One-Liner pour tout réinstaller

```powershell
# Créer dossiers
mkdir C:\mosquitto\config, C:\mosquitto\data, C:\mosquitto\log -Force

# Créer config
@"
listener 1883 0.0.0.0
allow_anonymous true
persistence true
persistence_location /mosquitto/data/
log_dest file /mosquitto/log/mosquitto.log
log_dest stdout
"@ | Out-File -FilePath C:\mosquitto\config\mosquitto.conf -Encoding UTF8

# Lancer Docker
docker run -d --name mosquitto -p 1883:1883 -v C:\mosquitto\config\mosquitto.conf:/mosquitto/config/mosquitto.conf -v C:\mosquitto\data:/mosquitto/data -v C:\mosquitto\log:/mosquitto/log --restart unless-stopped eclipse-mosquitto

# Afficher l'IP
ipconfig | Select-String "IPv4"
```

---

## 📝 Notes Importantes

- ⚠️ **IP du broker** : Toujours utiliser l'IP de VOTRE PC, pas celle de l'ESP32
- ⚠️ **0.0.0.0** : Obligatoire dans `mosquitto.conf` pour accepter les connexions externes
- ⚠️ **Firewall** : Peut bloquer le port 1883 (désactiver temporairement pour tester)
- ⚠️ **Même réseau** : ESP32 et PC doivent être sur le même WiFi
- 💡 **LED_PIN** : Par défaut GPIO2 (LED intégrée sur la plupart des ESP32)

---

## 🎯 Checklist de démarrage

- [ ] Docker Desktop lancé
- [ ] Mosquitto container en cours (`docker ps`)
- [ ] IP du PC trouvée (`ipconfig`)
- [ ] IP dans `config.h` mise à jour
- [ ] Code uploadé sur ESP32
- [ ] Serial Monitor ouvert
- [ ] ESP32 connecté au WiFi
- [ ] ESP32 connecté au broker MQTT
- [ ] Messages reçus dans `mosquitto_sub`

---

**Temps total d'installation : ~5 minutes** ⏱