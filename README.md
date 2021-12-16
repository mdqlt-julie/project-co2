# Projet Détecteur de CO²

Ce projet est inspiré du projet appartenant à [Grégoire RINOFLI](https://co2.rinolfi.ch/)

## Rappel

La modèle du  **capteur co²** est : ***Senseair S8***.
Voici différent document à propos du capteur :

* [Page acceuil du capteur](https://senseair.com/products/size-counts/s8-residential/)
* [Fiche Technique Modbus du capteur](https://rmtplusstoragesenseair.blob.core.windows.net/docs/Dev/publicerat/TDE2067.pdf)
* [Différentes sources sur Modbus.org](https://modbus.org/specs.php)

Pour le moment n'ayant pas accès au capteur, j'émulerais le capteur avec une carte **Arduino**.

Nous utilison également une carte M5Stack Atom qui dialoguera avec le capteur.

Voici les ressources qui concerne cette carte :

* [Page constructeur du produit.](https://shop.m5stack.com/collections/atom-series/products/atom-matrix-esp32-development-kit)

* [Documentation sur la gestion de la carte avec Arduino IDE](https://docs.m5stack.com/en/quick_start/m5core/arduino)

## Étudologie

La première étape va concister à créer un lien physique entre la **carte** et le **capteur CO²**
Pour le moment, les cartes ne fonts pas grand chose

* la **carte** envoie la requet *Ping!* au **capteur CO²**
* le **capteur CO²** renvoie à son tour la réponse *Pong!*

### Prochaine étape

La prochaine étape va consister à décortiquer le fonctionement de Modbus.
Voir le dossier ***Modbus*** qui est compiler avec gcc sous cygwin

Le protocole modbus permet deux modes de transmission de bites (Le mode ***RTU*** et ***ASCII***).

Notre capteur utilise le mode de transmission ***RTU***.
