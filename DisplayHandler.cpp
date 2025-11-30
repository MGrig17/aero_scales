  #include "DisplayHandler.h"
//   #include <LiquidCrystal_I2C.h> -- уже есть в "DisplayHandler.h"
// диспетчер команд.

DisplayHandler displayHandler;

DisplayHandler::DisplayHandler() 
//     : lcd(LiquidCrystal_I2C(LCD_I2C_ADDRESS, 16, 2)) {  // Прямая инициализация
// }
    : lcdWeights(LiquidCrystal_I2C(LCD_I2C_ADDRESS_1, 16, 2)),
      lcdAngle(LiquidCrystal_I2C(LCD_I2C_ADDRESS_2, 16, 2)) {
}

void DisplayHandler::begin() {
    Wire.begin();

    // lcd.init();
    // lcd.backlight();
    // lcd.clear();
    // lcd.print("Angle: "); 

    // Инициализация дисплея для весов
    lcdWeights.init();
    lcdWeights.backlight();
    lcdWeights.clear();
    lcdWeights.setCursor(0, 0);
    lcdWeights.print("1TD: 0.0g");
    lcdWeights.setCursor(0, 1);
    lcdWeights.print("2TD: 0.0g");
    
    // Инициализация дисплея для угла
    lcdAngle.init();
    lcdAngle.backlight();
    lcdAngle.clear();
    lcdAngle.setCursor(0, 0);
    lcdAngle.print("Angle: ");
}

void DisplayHandler::displayWeights(float weight1, float weight2) {
    
    // ВОССТАНАВЛИВАЕМ подписи если они пропали
    lcdWeights.setCursor(0, 0);
    lcdWeights.print("1TD:");
    lcdWeights.setCursor(0, 1);
    lcdWeights.print("2TD:");

    // ТОЛЬКО на дисплей весов 
    lcdWeights.setCursor(5, 0);  // После "1TD: "
    lcdWeights.print(weight1, 1);
    lcdWeights.print("g   ");
    
    lcdWeights.setCursor(5, 1);  // После "2TD: "
    lcdWeights.print(weight2, 1);
    lcdWeights.print("g   ");
}

void DisplayHandler::displayAngle(float angle) {
    // ВОССТАНАВЛИВАЕМ подпись если она пропала
    lcdAngle.setCursor(0, 0);
    lcdAngle.print("Angle:");

    // ТОЛЬКО на дисплей угла 
    lcdAngle.setCursor(7, 0);  // После "Angle: "
    lcdAngle.print(angle, 1); //////////////////////////////////////////////////////////////смена знака
    lcdAngle.print((char)223); 
    lcdAngle.print("   ");     
}

void DisplayHandler::displayNoMagnet() {

    // ВОССТАНАВЛИВАЕМ подпись
    lcdAngle.setCursor(0, 0);
    lcdAngle.print("Angle:");
    
    // Сообщение об отсутствии магнита
    lcdAngle.setCursor(7, 0);
    lcdAngle.print("No Magnet!    ");
}

void DisplayHandler::displayMessage(const String& line1, const String& line2) {
    clear(); // ← ИСПОЛЬЗУЕМ СУЩЕСТВУЮЩИЙ МЕТОД
    lcdAngle.setCursor(0, 0);
    lcdAngle.print(line1);
    lcdAngle.setCursor(0, 1);
    lcdAngle.print(line2);
}
// очистка дисплея 
void DisplayHandler::clear() {
    lcdWeights.clear();
    lcdAngle.clear();
}
