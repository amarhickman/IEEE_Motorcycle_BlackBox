#include <SPI.h>
#include <SD.h>

const int SD_CS = D10;

void setup() {
  Serial.begin(115200);

  // Allow time to open Serial Monitor.
  unsigned long start = millis();
  while (!Serial && millis() - start < 5000) {
    delay(10);
  }

  Serial.println("\nStarting SD card test...");

  // Order: SCK, MISO, MOSI, CS
  SPI.begin(D13, D12, D11, SD_CS);

  // Start slowly for reliable breadboard testing.
  if (!SD.begin(SD_CS, SPI, 1000000)) {
    Serial.println("FAIL: SD card initialization.");
    Serial.println("Check wiring, module power, and card format.");
    return;
  }

  Serial.println("PASS: SD card initialized.");

  // Append a new line without deleting existing contents.
  File file = SD.open("/nano_test.txt", FILE_APPEND);

  if (!file) {
    Serial.println("FAIL: Could not open file for writing.");
    return;
  }

  size_t written = file.println("Hello from the Nano ESP32!");
  file.close();

  if (written == 0) {
    Serial.println("FAIL: Could not write to the file.");
    return;
  }

  Serial.println("PASS: Wrote to nano_test.txt.");

  file = SD.open("/nano_test.txt", FILE_READ);

  if (!file) {
    Serial.println("FAIL: Could not open file for reading.");
    return;
  }

  Serial.println("\nFile contents:");

  while (file.available()) {
    Serial.write(file.read());
  }

  file.close();
  SD.end();

  Serial.println("\nSD card test complete.");
}

void loop() {
}