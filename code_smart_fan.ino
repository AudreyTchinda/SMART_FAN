//gession des bouton pousoir
boolean bp_vitesse_plus, bp_vitesse_moin, bp_veilleuse, bp_difuseur;
boolean bp_vit_plus_pres, bp_vit_moin_pres, bp_veille_pres, bp_dif_pres;
int bp[4] = { 5, 6, 7, 8 };
//gession des leds
int led[3] = { 2, 3, 4 };
//gession des appareils de sortie
int sorti[3] = { 10, 11, 12 };                                       // 3 sorties 10: moteur 11: veilleuse 12:diffuseur
int i, dif_on_off, vits;                                             // i:indicateur de vitesse du moteur , dif_on_off: indicateur de l'etat du diffuseur
boolean on_off, b, c, d, tm, f, e, on_timer, timer_veil, timer_dif;  //v_plus_ir ,v_moins_ir : indicateur de vitesse du moteur pour la telecommande on_off: marche et arret du systeme pour la telecommande
int v_plus_ir, v_moins_ir, veil_ir, dif_ir;
#include <IRremote.h>                                                //bibliotheque IR
const char DIN_RECEPTEUR_INFRAROUGE = 14;                            // broche du signal IR 9
unsigned long temps, temps_timer, temps_time, temps_dif, tem_timer;  // variable pour le temps de marche et d'arret du diffuseur
IRrecv monRecepteurInfraRouge(DIN_RECEPTEUR_INFRAROUGE);             // indication de la broche de connection du ir

decode_results messageRecu;  //variable de stockage du message de IR
void setup() {
  Serial.begin(9600);
  /* initialisation des broches de sorties*/
  for (i = 0; i <= 4; i++) {
    Serial.println(i);
    pinMode(bp[i], INPUT);
    if (i < 4) {
      pinMode(led[i], OUTPUT);
      digitalWrite(led[i], 1);
      delay(200);
      digitalWrite(led[i], 0);
      pinMode(sorti[i], OUTPUT);
    }
    pinMode(9,OUTPUT);
    ///////////////////////////////////////
    /*initialisation de IR*/
    monRecepteurInfraRouge.enableIRIn();
    monRecepteurInfraRouge.blink13(true);
    ////////////////////////////////
  }
  Serial.print("ok");
  i = 0;
  on_off = 1;
  d = 0;
}

void loop() {
  irRead();
  /*
    condition N1 sur on_off permet à la telecommande de metre en marche ou en arret le systeme
        condition N2 permet de regarder si l'utilisateur veut augmenter la vitesse
        condition N3 permet de regarder si l'utilisateur veut diminuer la vitesse
        condition N4 permet de regarder si l'utilisateur veut oui ou non allumer la veilleuse
        condition N4 permet de regarder si l'utilisateur veut oui ou non allumer la le diffuseur
    les 4 dernieres lignes permetent de sauvegarder l'état precedent des boutons poursoi
  */
  if (on_off == 1) {
    //timerr();
    if (dif_on_off == 1) diff();
    bp_vitesse_plus = digitalRead(bp[0]);
    bp_vitesse_moin = digitalRead(bp[1]);
    bp_veilleuse = digitalRead(bp[2]);
    bp_difuseur = digitalRead(bp[3]);
    if ((i >= 3) and (v_plus_ir == 1)) { v_plus_ir = 0; }
    if ((i == 0) and (v_moins_ir == 1)) { v_moins_ir = 0; }

    if ((((bp_vitesse_plus == 1) and (bp_vit_plus_pres == 0)) or (v_plus_ir == 1) /*and (millis() > (tem_timer + 1000))*/) and (i < 3)) {
      i += 1;
      v_plus_ir = 0;
      Serial.println("v+");
      vit();
      digitalWrite(9, !digitalRead(9));
      digitalWrite(led[0], !digitalRead(led[0]));
    }
    if ((((bp_vitesse_moin == 1) and (bp_vit_moin_pres == 0)) or (v_moins_ir == 1) /*and (millis() > (tem_timer + 1000))*/) and (i > 0)) {
      i -= 1;
      v_moins_ir = 0;
      Serial.println("v-");
      vit();
      digitalWrite(9, !digitalRead(9));
      digitalWrite(led[0], !digitalRead(led[0]));
    }
    if (((bp_veilleuse == 1) and (bp_veille_pres == 0)) or (veil_ir == 1) /*and (millis() > (tem_timer + 1000))*/) {
      veil_ir = 0;
      digitalWrite(sorti[1], !digitalRead(sorti[1]));
      digitalWrite(led[1], !digitalRead(led[1]));
      Serial.println("veille");
    }
    if (((bp_difuseur == 1) and (bp_dif_pres == 0)) or (dif_ir == 1) /*and (millis() > (tem_timer + 1000))*/) {
      dif_ir = 0;
      dif_on_off += 1;
      digitalWrite(led[2], !digitalRead(led[2]));
      diff();
      // Serial.println("dif");
    }
    bp_vit_plus_pres = bp_vitesse_plus;
    bp_vit_moin_pres = bp_vitesse_moin;
    bp_veille_pres = bp_veilleuse;
    bp_dif_pres = bp_difuseur;
  }
  // put your main code here, to run repeatedly:
}
void vit() {
  /*procédure de gestion des vitesses*/
  if (i == 0) {
    while (vits > 0) {
      vits -= 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
  }
  if (i == 1) {
    while (vits > 40) {
      vits -= 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
    while (vits < 40) {
      vits += 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
  }
  if (i == 2) {
    while (vits > 80) {
      vits -= 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
    while (vits < 80) {
      vits += 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
  }
  if (i == 3) {
    while (vits < 170) {
      vits += 5;
      analogWrite(sorti[0], vits);
      delay(120);
    }
  }
}
void diff() {
  /*procédure de gestion du diffuseur : mise en marche du diffuseur en marche lors que
    dif_on_off ets à 1 arret lors qu'il est à 2  */

  //Serial.println(i);
  if ((dif_on_off == 2)) {
    // Serial.print(c);
    if (c == 0) {
      digitalWrite(sorti[2], 1);
      delay(10);
      digitalWrite(sorti[2], 0);
      delay(200);
      digitalWrite(sorti[2], 1);
      delay(70);
      digitalWrite(sorti[2], 0);
      //delay(70);
      delay(200);
      digitalWrite(sorti[2], 1);
      delay(70);
      digitalWrite(sorti[2], 0);
      delay(70);
      Serial.println("no_dif_ok");
    }
    Serial.println("no_dif_pres");
    dif_on_off = 0;
    c = 0;
    f = 0;
    e = 0;
  }

  if (dif_on_off == 1) {
    /* digitalWrite(sorti[2], HIGH);
      delay(60);
      digitalWrite(sorti[2], LOW);
      delay(60);
      digitalWrite(sorti[2], HIGH);
      delay(60);
      digitalWrite(sorti[2], LOW);*/

    if (((c == 1) and (millis() > (temps_dif + 40000))) or ((f == 0) and (c == 0))) {
      temps_dif = millis();
      if (f == 0) c = 1;
      digitalWrite(sorti[2], HIGH);
      delay(60);
      digitalWrite(sorti[2], LOW);
      delay(200);
      digitalWrite(sorti[2], HIGH);
      delay(60);
      digitalWrite(sorti[2], LOW);
      Serial.println("dif_1");
      e = 1;
      c = !c;
      f = 1;
    } else {
      if ((millis() > (temps_dif + 10000)) and (c == 0)) {
        digitalWrite(sorti[2], HIGH);
        delay(70);
        digitalWrite(sorti[2], LOW);
        delay(200);
        digitalWrite(sorti[2], HIGH);
        delay(70);
        digitalWrite(sorti[2], LOW);
        Serial.println("no_dif_1");
        c = !c;
        e = 0;
      }
    }
    //Serial.println(c);
  }
}
void irRead() {
  /*reception et traitement des données recu de l'IR*/
  if (monRecepteurInfraRouge.decode(&messageRecu)) {
    Serial.println(messageRecu.value, HEX);
    delay(500);
    monRecepteurInfraRouge.resume();
    if (messageRecu.value == 0x371A3C86) {
      /* mise en veille du systeme ou sortie du mode veille par l'IR*/
      on_off = !on_off;
      b = !b;
      if (b == 1) {
        //mise en veille du systeme
        timer_veil = 1;
        timer_dif = 1;
        veille();
      }
      /////////////////////////////////////////////////

      //mise hort veille du systeme
      if (on_off == 1) {
        digitalWrite(led[2], 1);
        delay(200);
        digitalWrite(led[1], 1);
        delay(200);
        digitalWrite(9,1);
        digitalWrite(led[0], 1);
        delay(200);
         digitalWrite(9,0);
        digitalWrite(led[0], 0);
        digitalWrite(led[1], 0);
        digitalWrite(led[2], 0);
      }
      //////////////////////////////////////////I
      b = 0;
    }
    //traitement des données recu de IR
    if (on_off == 1) {
      if (messageRecu.value == 0xE0984BB6) {
        v_plus_ir = 1;  //tem_timer = millis();
        Serial.println(v_plus_ir);
        /* if (v_plus_ir == 2) {
          int timess;
          if (tm == 0) {
            temps_timer = millis();
            Serial.println("timer_on");
            digitalWrite(led[0], !digitalRead(led[0]));
          }
          if (temps_time < 14400000) {
            temps_time += 1800000;
            timess = temps_time / 60000;
            Serial.println("time_m :");
            Serial.println(timess);
            tm = 1;
          }
          v_plus_ir = 0;
        }*/
      }
      if (messageRecu.value == 0xA32AB931) {
        v_moins_ir = 1;
        /* tem_timer = millis();
        Serial.println(v_moins_ir);
        if (v_moins_ir == 2) {
          int timess;
          if (temps_time <= 0) {
            tm = 0;
            temps_time = 0;
          }
          if (temps_time > 0) {
            if (tm == 0) {
              temps_timer = millis();
              Serial.println("timer_on");
              digitalWrite(led[0], !digitalRead(led[0]));
            }
            temps_time -= 1800000;
            timess = temps_time / 60000;
            Serial.println("time_m :");
            Serial.println(timess);
            tm = 1;
          }
          v_moins_ir = 0;
        }*/
      }
      if (messageRecu.value == 0x39D41DC6) {
        veil_ir += 1;
        /* tem_timer = millis();
        if (veil_ir == 2) {
          if (tm == 1) {
            timer_veil = !timer_veil;
                digitalWrite(led[1], !digitalRead(led[1]));
           
          }
          veil_ir = 0;
        }*/
      }
      if (messageRecu.value == 0x143226DB) {
        dif_ir += 1;
        /*tem_timer = millis();
        if (dif_ir == 2)
        { if (tm == 1) {
            timer_dif = !timer_dif;
                 digitalWrite(led[2], !digitalRead(led[2]));
          }
          dif_ir = 0;
        }
      }*/
      }
      delay(1);
    }
  }
  /*void timerr() {
  if ((tm == 1) and (millis() > (temps_timer + temps_time))) {
  if((timer_veil==1) and (timer_dif==1)){  on_off = 0;
    veille();}
    analogWrite(sorti[0], 0);
    i=0;
    if(timer_dif==1){timer_dif=0; tm=0; if (e == 1) {
      digitalWrite(sorti[2], HIGH);
      delay(70);
      digitalWrite(sorti[2], LOW);
      delay(70);
      Serial.print("veillllll");
      c = 0; f = 0; e=0;
      dif_on_off = 0;
    }}
    if(timer_veil==1){timer_veil=0; analogWrite(sorti[0], 0);}
    tm = 0;
    temps_time = 0;
  }*/
}
void veille() {
  if (on_off == 0) {

    digitalWrite(led[0], 1);
    digitalWrite(led[1], 1);
    digitalWrite(led[2], 1);
    delay(50);
    digitalWrite(led[0], 0);
    delay(50);
    digitalWrite(led[1], 0);
    delay(50);
    digitalWrite(led[2], 0);
    i = 0;
    analogWrite(sorti[0], 0);
    if (timer_veil == 1) digitalWrite(sorti[1], 0);
    delay(50);
    if (e == 1) {
      digitalWrite(sorti[2], 1);
      delay(10);
      digitalWrite(sorti[2], 0);
      delay(200);
      digitalWrite(sorti[2], 1);
      delay(70);
      digitalWrite(sorti[2], 0);
      //delay(70);
      delay(200);
      digitalWrite(sorti[2], 1);
      delay(70);
      digitalWrite(sorti[2], 0);
      delay(70);
      Serial.println("no_dif_ok");
      Serial.print("veillllll");
      c = 0;
      f = 0;
      e = 0;
      dif_on_off = 0;
    }
    timer_dif = 0;
    Serial.print("veillllll");
  }
}