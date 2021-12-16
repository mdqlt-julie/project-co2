/**
 * @file config.h
 * @author Julie Brindejont (julie.brindejont)
 * @brief 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 * Fichier 'config.h' faisant partie du projet 'détecteur de CO²' à destination de la mainson de quartier la touche.
 * 
 * Ce fichier contient les entêtes et paramètres essentiels pour le bon déroulement du projet.
 */

#ifndef CONFIG_H
#define CONFIG_H

//Je vais inclure ici ma librairie "ArduinoWrapper" créer par mes soins.
//Je trouve que cela me facilite la gestion d'arduino.
#include <ArduinoWrapper.h>


// ==============================================================================================
// ==                                                                                          ==
// ==                                      CONSTANTES                                          ==
// ==                                                                                          ==
// ==============================================================================================
#define PIN_LED_RED    25
#define PIN_LED_GREEN  21
#define PIN_LED_ORANGE 21
#define PIN_SERIAL_RX  33
#define PIN_SERIAL_TX  23
// ==============================================================================================
// ==                                                                                          ==
// ==                               PROTOTYPE DES FONCTIONS                                    ==
// ==                                                                                          ==
// ==============================================================================================

/**
 * @brief Fonction nécessaire pour l'initialisation de l'arduino
 * Dans notre cas cette fonction initialisera paramettre le 'capteur'
 */
void setup();

/**
 * @brief Fonction nécessaire qui est appelé en boucle par le programme après le passage dans la fonction setupe
 * Un peut comme le jeu du ping pong, le 'capteur' sera en écoute, et attend ces instruction.
 */
void loop();


#endif