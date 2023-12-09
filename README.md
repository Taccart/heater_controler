# Repair week-end :  radiateur de salle de bains.

## Constat
La console de mon radiateur soufflant de salle de bains affiche un code d' erreur non documenté. 
J' ai changé deux fois un condensateur defaillant sur la carte de controle.
La partie éléctronique dépasse mes compétences.
La partie électrique estassez simple :
* deux bornes pour la partie chauffage
* deux bornes pour la partie soufflante.

## Objectif
Réparer le radiateur en remplaçant la partie électronique.

### Composants
- un arduino compatible
- un capteur de temperature & hygrometrie
- un afficheur 
- des boutons
- deux relais 

### Logiciel
En version 1, le nouveau controleur devra permettre de 
- regler le thermostat entre 0°C et 25°C. 0°C sera considéré comme un arrêt du chauffage.
- mettre en route la ventilation pour 15 minutes.
L' écran devra afficher
- la temperature actuelle
- l' hygrometrie actuelle
- la temperature cible
- l' etat en chauffe ou pas
- le compte a rebours de la ventillation 

Plus tard, si tout va bien:
* programmation de la mise en route de la ventilation (heure-minute, jour de semaine...)
* programmation du thermostat (ex: chauffer entre 4h et 6h à 20°, et à 16°C le reste du temps.)
* lecture informations via wifi
* interactions via wifi 


## Version 1
### Composants
- Un ESP8266 --> [WEMOS Mini D1 Pro ](https://www.amazon.fr/Connecteur-dantenne-ESP8266EX-d%C3%A9veloppement-128MMBIT/dp/B0CJHWGHK7/ref=sr_1_7?keywords=wemos+d1+mini+pro&qid=1702125536&sr=8-7)
- Un écran LCD RGB avec 5 boutons poussoirs -->  [RGB LCD Shield Kit](https://www.adafruit.com/product/716)
- Un double relais 220V 30A (pour avoir de la marque) --> [30 A 2 canaux](https://www.amazon.fr/dp/B07QN5H6B1?psc=1&ref=ppx_yo2ov_dt_b_product_details)

### Connections
Les +5V et GND ne sont pas représentés 
```
                             ╔══════════╗
                             ║DHT22     ║
                             ║══════════║
                             ║+5V       ║
 ╔══════════════╗            ║GND       ║
 ║WemosD1MiniPro║   ┌────────╢pin_SENSOR║
 ║══════════════║   │        ╚══════════╝
 ║+5V           ║   │        ╔═══════╗
 ║GND           ║   │        ║Relay  ║
 ║D0            ╟───┘        ║═══════║
 ║D3            ╟────────┐   ║+5V    ║
 ║D4            ╟──────┐ │   ║GND    ║
 ║SDA           ╟────┐ │ └───╢pin_A  ║
 ║SCL           ╟──┐ │ └─────╢pin_B  ║
 ╚══════════════╝  │ │       ╚═══════╝
                   │ │       ╔════════╗
                   │ │       ║LCD1602 ║
                   │ │       ║════════║
                   │ │       ║+5V     ║
                   │ │       ║GND     ║
                   │ └───────╢pin_SDA ║
                   └─────────╢pin_SCL ║
                             ╚════════╝

```


