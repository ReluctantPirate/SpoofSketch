enum signalStates {SETUP, HIDE, REVEAL};
byte signalState = SETUP;

bool isOn = false;

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
      if (previewTimer.isExpired()) {
        hideDisplay();
      } else {
        revealDisplay();
      }
      break;
    case REVEAL:
      revealLoop();
      revealDisplay();
      break;
  }

  setValueSentOnAllFaces(signalState);

  buttonSingleClicked();
  buttonDoubleClicked();
}

void setupLoop() {

  if (buttonSingleClicked()) {
    isOn = !isOn;
  }

  if (buttonDoubleClicked()) {
    signalState = HIDE;
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      if (getLastValueReceivedOnFace(f) == HIDE) {
        signalState = HIDE;
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

  if (buttonSingleClicked() && isOn) {
    previewTimer.set(1000);
  }
}

void revealLoop() {
  if (buttonDoubleClicked()) {
    signalState = SETUP;
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      if (getLastValueReceivedOnFace(f) == SETUP) {
        signalState = SETUP;
      }
    }
  }
}

void setupDisplay() {
  setColor(dim(CYAN, 50));
  if (isOn) {
    setColorOnFace(dim(WHITE, 150), random(5));
    setColorOnFace(dim(WHITE, 150), random(5));
    setColorOnFace(dim(WHITE, 150), random(5));
  } else {
    setColorOnFace(OFF, random(5));
    setColorOnFace(OFF, random(5));
    setColorOnFace(OFF, random(5));
  }
}

void hideDisplay() {
  FOREACH_FACE(f) {
    setColorOnFace(dim(YELLOW, random(100)), f);
  }
}

void revealDisplay() {
  if (isOn) {
    setColor(GREEN);
    setColorOnFace(OFF, random(5));
  } else {
    setColor(OFF);
    setColorOnFace(dim(GREEN, 100), random(5));
  }
}
