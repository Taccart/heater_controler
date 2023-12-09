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
- Un ESP8266 (Wemos D1 mini pro) que j'avais sous la main, qui me facilitera la 
- Un écran LCD RGB avec 5 boutons poussoirs
- Un double relais 220V 30A (pour avoir de la marque)

### Connections
Les +5V et GND ne sont pas représentés 
```                          ╔══════════╗
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


