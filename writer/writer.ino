#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  D8
#define RST_PIN D3

#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxime a tag...");
  Serial.println();
}

void loop() {

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  int option = menu();

  if (option == 0) 
    readData();
  else if(option == 1) 
    writeData();
  else {
    Serial.println(F("Opção inválida."));
    return;
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void readData()
{
  // Escreve o UID da tag
  Serial.print(F("Tag UID: "));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.print(F(" "));
  
  // Prepara a chave para o padrão de fábrica
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Define em qual bloco será feita a leitura.
  byte buffer[SIZE_BUFFER] = {0};
  byte block = 1;
  byte blockSize = SIZE_BUFFER;

  // Verifica a autenticação da tag
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na autenticação: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Verifica a leitura do bloco
  status = mfrc522.MIFARE_Read(block, buffer, &blockSize);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na leitura: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Imprime os dados lidos no bloco especificado
  Serial.print(F("\nDados do bloco ["));
  Serial.print(block);
  Serial.print(F("]: "));
  for (uint8_t i = 0; i < MAX_SIZE_BLOCK; i++)
  {
    Serial.write(buffer[i]);
  }
  Serial.println(" ");
}

void writeData()
{
  // Escreve o UID da tag
  Serial.print(F("Tag UID: "));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  
  // Aguarda a leitura por até 30s
  Serial.setTimeout(30000L);
  Serial.println(F("\nEntre com o dado com o carctere '#' ao final (máximo de 16 caracteres):"));

  // Prepara a chave para o padrão de fábrica
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Define em qual bloco será feita a escrita.
  byte buffer[MAX_SIZE_BLOCK] = "";
  byte block;
  byte dataSize;

  // Lê os dados até o caractere '#'
  dataSize = Serial.readBytesUntil('#', (char*)buffer, MAX_SIZE_BLOCK);
  for(byte i=dataSize; i < MAX_SIZE_BLOCK; i++)
  {
    buffer[i] = ' ';
  }

  block = 1;
  // Transforma os dados em string
  String str = (char*)buffer;
  Serial.println(str);

  // Verifica a autenticação da tag
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na autenticação: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  // Verifica a gravação no bloco
  status = mfrc522.MIFARE_Write(block, buffer, MAX_SIZE_BLOCK);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na escrita de dados: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    Serial.println(F("Dados escritos com sucesso!"));
  }
}

int menu()
{
  Serial.println(F("\nEntre com uma opção:"));
  Serial.println(F("0 - Ler"));
  Serial.println(F("1 - Escrever\n"));

  // Aguarda a entrada na serial
  while(!Serial.available()){};

  int op = (int)Serial.read();
  while(Serial.available()) {
    if (Serial.read() == '\n') break; 
    Serial.read();
  }
  return (op-48);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
