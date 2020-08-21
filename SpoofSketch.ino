enum signalStates {SETUP, HIDE, REVEAL};
byte signalState = SETUP;

bool isOn = false;

bool isChaos = false;

#define PREVIEW_TIME 1000
Timer previewTimer;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  switch (signalState) {
    case SETUP:
      setupLoop();
      setupDisplay();
      break;
    case HIDE:
      hideLoop();
      hideDisplay();
      break;
    case REVEAL:
      revealLoop();
      revealDisplay();
      break;
  }

  setValueSentOnAllFaces(signalState);

  buttonSingleClicked();
  buttonDoubleClicked();
  buttonMultiClicked();
}

void setupLoop() {

  if (buttonSingleClicked() && !isChaos) {
    isOn = !isOn;
  }

  if (buttonDoubleClicked()) {
    signalState = HIDE;
    if (isChaos) {
      isOn = random(1);
    }
  }

  if (buttonMultiClicked() && buttonClickCount() == 3) {
    isChaos = !isChaos;
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      if (getLastValueReceivedOnFace(f) == HIDE) {
        signalState = HIDE;
        if (isChaos) {
          isOn = random(1);
        }
      }
    }
  }
}

void hideLoop() {

  if (buttonDoubleClicked()) {
    signalState = REVEAL;
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      if (getLastValueReceivedOnFace(f) == REVEAL) {
        signalState = REVEAL;
      }
    }
  }

  if (buttonSingleClicked()) {
    previewTimer.set(PREVIEW_TIME);
  }
}

void revealLoop() {
  if (buttonDoubleClicked()) {
    signalState = SETUP;
    if (isChaos) {
      isOn = false;
    }
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      if (getLastValueReceivedOnFace(f) == SETUP) {
        signalState = SETUP;
        if (isChaos) {
          isOn = false;
        }
      }
    }
  }
}

void setupDisplay() {
  //set base colors
  setColor(YELLOW);
  setColorOnFace(dim(YELLOW, 150), 2);
  setColorOnFace(dim(YELLOW, 150), 3);
  setColorOnFace(dim(YELLOW, 100), 4);
  setColorOnFace(dim(YELLOW, 100), 5);

  if (isOn) {
    byte segmentLight = (millis() / 200) % 4;
    switch (segmentLight) {
      case 0:
        setColorOnFace(RED, 0);
        setColorOnFace(RED, 1);
        break;
      case 1:
        setColorOnFace(dim(RED, 150), 2);
        setColorOnFace(dim(RED, 150), 3);
        break;
      case 2:
        setColorOnFace(dim(RED, 100), 4);
        setColorOnFace(dim(RED, 100), 5);
        break;
    }
  }

  if (isChaos) {
    setColorOnFace(OFF, random(5));
  }
}

void hideDisplay() {
  setColor(OFF);
  byte segmentLight = (millis() / 1000) % 3;
  byte segmentProgress = map(millis() % 1000, 0, 1000, 0, 255);
  setColorOnFace(dim(YELLOW, segmentProgress), segmentLight * 2);
  setColorOnFace(dim(YELLOW, segmentProgress), (segmentLight * 2) + 1);

  if (!previewTimer.isExpired()) {
    if (isOn) {
      setColorOnFace(RED, random(5));
      setColorOnFace(RED, random(5));
    } else {
      setColorOnFace(CYAN, random(5));
      setColorOnFace(CYAN, random(5));
    }
  }
}

void revealDisplay() {
  setColor(OFF);
  if (isOn) {
    setColorOnFace(RED, random(5));
    setColorOnFace(RED, random(5));
  } else {
    setColorOnFace(CYAN, random(5));
    setColorOnFace(CYAN, random(5));
  }
}
