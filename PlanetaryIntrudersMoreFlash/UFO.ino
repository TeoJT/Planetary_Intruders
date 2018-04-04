
void UFOAppear() {
  for (byte AppearingPos = 0; AppearingPos < 25; AppearingPos++) { //Make the UFO hover down while playing a woozy sound.
    DisplayInfo(WHITE);
    Render(56, AppearingPos, 14, WHITE);  //UFO appearing sound removed in this version of Planetary Intruders.
    LOAD;
    delay(100);
    Render(56, AppearingPos, 14, BLACK);
  }

  //Set up the UFO.
  UFOPoints = 0;
  UFOHP = Level * 8;
  UFOHasBombs = 0;
  EnemyCount = 9;
  UFOX = 56;
  UFOY = 16;
  delay(600);
}

void RunUFOCode() {
  static unsigned char UFOHoverMovement = 1;
  static byte UFOHIT = 0;            //Create static local variables.
  static byte UFOShoot = 250;
  static byte UFOGlideTo = 20;
    if (UFOHP) {
      PlayMusic(2);          //Play the music.
    }
    Render(UFOX, UFOY, 7, BLACK);

  if (!Freeze) {
    if (UFOPoints < 200) {
      UFOX += UFOHoverMovement;
      if ((UFOX > 60) || (UFOX < 52)) { //If UFO hasn't obtained enough aliens, just hover like normal.
        UFOHoverMovement *= -1;
      }
    }
    else {
          UFOX += UFOHoverMovement;  //Otherwise move all around the place, particularly to where the player is.
      if (UFOHP) {
          UFOX += UFOHoverMovement;
          if (UFOX < 4) {
            UFOX += 3;
          }
          if (UFOX > 108) {
            UFOX -= 3;
          }
          if ((UFOX + 4) > PlayerX) {
            UFOX--;
          }
          if ((UFOX + 4) < PlayerX) {
            UFOX++;
          }
        if (UFOY < UFOGlideTo) {
          UFOY++;
        }
        if (UFOY > UFOGlideTo) {
          UFOY--;
        }
        if (UFOY == UFOGlideTo) {
          UFOGlideTo = random(35);
          UFOHoverMovement *= -1;
        }
      }
    }
    UFOShoot -= 2;   //Countdown UFOShoot.
    if ((UFOShoot <= UFOPoints) && UFOHP) {  //If UFOShoot reaches the value of UFOPoints, set UFOShoot 226 and fire a laser (or bomb).
      UFOShoot = 226;                        //That means the more points the UFO has, the shorter the UFOShoot timer will be, thus the
      SpawnLaser(UFOX + 8, 21, UFOHasBombs); //faster the UFO will shoot lasers.
      if ((UFOPoints >= 200) && (random(5) == 4)) {  //If UFOPoints is over 200, occasionally shoot two extra lasers.
        SpawnLaser(UFOX + 2, 21, 0);
        SpawnLaser(UFOX + 14, 21, 0);
      }
    }
  }
    
    
    if (UFOHIT) {
      Render(UFOX, UFOY, 8, WHITE);   //Flash if hit by laser.
      UFOHIT--;
    }
    else {
      Render(UFOX, UFOY, 7, WHITE);   //Otherwise, render normally.
    }
    if (!UFOHP) {
      Render(UFOX, UFOY, 7, BLACK);   //Make the UFO disappear when defeated and the defeat animation is over.
    }

    if (MegaShotCharge == 254) {      //If the player uses the mega shot power up, the UFO takes damage based on its HP.
      UFOHIT = 10;
      UFOHP -= Level * 2;
      if (UFOHP <= 0) {    //And if the UFO is defeated by mega shot, trigger the death animation.
        Freeze = 0;
        UFOHP = 0;
      }
    }
    


    
    for (byte I = 0; I < MaxPlayerLasers; I++) {   //Detect if a laser has hit the UFO.
      if ((PlayerLaserY[I] < UFOY + 8) && (PlayerLaserY[I] > UFOY) && (PlayerLaserX[I] >= UFOX) && (PlayerLaserX[I] <= (UFOX + 16)) && UFOHP) {
        SOUND(1, 277, 1);
        if (random(0, 201 + UFOPoints) <= 200) {
          UFOHP--;
          if (!UFOHP) {
            Freeze = 0;
          }
        }
        UFOHIT = 10;
        DeletePlayerLaser(I);
      }
    }

    //UFO death animation. Wait no, it's not an animation. The UFO just goes *poof* and then it's gone in this version.
    if (!UFOHP) {
        ExplosionSound = 15;
        Blast(UFOX + 8, 20);
        Score += Level * 80 + UFOPoints;
        UFOPoints = 0;
        NewLevel(0);
    }
}

