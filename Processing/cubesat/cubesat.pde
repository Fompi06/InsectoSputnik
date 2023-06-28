import processing.serial.*;
import controlP5.*;
ControlP5 cp5;
//Meter FiveV;
//Meter ThreeV;
//Meter Ampere;
String portName;     // имя порта
int speed = 9600;

Serial serial = null;
String myString = null;
int val = 0;
boolean skip = true;
String textValue = "";
Chart myChart;
Chart myChart2;
Chart myChart3;
Chart myChart4;
Chart myChart5;
Chart myChart6;
Chart myChart7;
Chart myChart8;
Chart myChart9;
Chart myChart10;
Textlabel tempLabel;
Textlabel tempBatLabel;
Textlabel tempCamLabel;
Textlabel tempBarLabel;
Textlabel presLabel;
Textlabel altLabel;
Textlabel altRelLabel;
Textlabel sputnikLabel;
Textlabel gpsAltLabel;
Textlabel longLabel;
Textlabel latiLabel;
Textlabel bar2Label;
Textlabel Time;
Textlabel Date;
Textlabel SenHead;
Textlabel GPSHead;
Textlabel CtrlHead;
Textlabel SerialHead;
Textlabel VHead;
Textlabel Monitor;
Textlabel SendLog;
Textlabel OnlineDelay;
Textarea myTextarea;
int c = 0;
Println console;

void setup()
{
  size(1100, 1025);
  //String portName = Serial.list()[2];

  delay(1000);
  frameRate(60);
  setupGUI();
  smooth(8);
}

void setupGUI() {
  cp5 = new ControlP5(this);
  cp5.enableShortcuts();
  //cp5.setFont(createFont("Agency FB", 20));  // сделаем шрифт побольше
  cp5.setFont(createFont("Tahoma", 15));  // сделаем шрифт побольше

  // выпадающий список
  cp5.addScrollableList("com")
    .setPosition(10, 10)
    .setSize(80, 100)
    .setBarHeight(30)
    .setItemHeight(30)
    .close()
    .addItems(Serial.list());
  ;

  // добавляем кнопки
  cp5.addButton("open").setPosition(90, 10).setSize(80, 30);
  cp5.addButton("close").setPosition(170, 10).setSize(80, 30);

  // ****************************ТЕМПЕРАТУРА****************************

  SenHead = cp5.addTextlabel("SenHead")
    .setText("Показания датчиков")
    .setPosition(186, 93)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("BebasNeue-Regular.otf", 32))
    ;


  myChart = cp5.addChart("tempC")
    .setPosition(20, 170)
    .setSize(200, 100)
    .setRange(-50, 40)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart.addDataSet("tempC");
  myChart.setData("tempC", new float[100]);

  cp5.addNumberbox("tempN")
    .setPosition(240, 170)
    .setSize(60, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  tempLabel = cp5.addTextlabel("tempLabel")
    .setText("Температура платы (°C)")
    .setPosition(20, 140)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;


  myChart2 = cp5.addChart("tempBatC")
    .setPosition(20, 310)
    .setSize(200, 100)
    .setRange(-20, 60)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart2.addDataSet("tempBatC");
  myChart2.setData("tempBatC", new float[100]);

  cp5.addNumberbox("tempBatN")
    .setPosition(240, 310)
    .setSize(60, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  tempBatLabel = cp5.addTextlabel("tempBatLabel")
    .setText("Температура аккумулятора (°C)")
    .setPosition(20, 280)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myChart3 = cp5.addChart("tempCamC")
    .setPosition(20, 450)
    .setSize(200, 100)
    .setRange(-20, 50)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart3.addDataSet("tempCamC");
  myChart3.setData("tempCamC", new float[100]);

  cp5.addNumberbox("tempCamN")
    .setPosition(240, 450)
    .setSize(60, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  tempCamLabel = cp5.addTextlabel("tempCamLabel")
    .setText("Температура в камере (°C)")
    .setPosition(20, 420)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;


  myChart4 = cp5.addChart("tempBarC")
    .setPosition(20, 170 + 140 * 3)
    .setSize(200, 100)
    .setRange(-50, 40)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart4.addDataSet("tempBarC");
  myChart4.setData("tempBarC", new float[100]);

  cp5.addNumberbox("tempBarN")
    .setPosition(240, 170 + 140 * 3)
    .setSize(60, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  tempBarLabel = cp5.addTextlabel("tempBar")
    .setText("Температура барометра (°C)")
    .setPosition(20, 140 + 140 * 3)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  //****************************ДАВЛЕНИЕ****************************

  myChart5 = cp5.addChart("presC")
    .setPosition(320, 170 + 140 * 0)
    .setSize(200, 100)
    .setRange(1000, 150000)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart5.addDataSet("presC");
  myChart5.setData("presC", new float[100]);

  cp5.addNumberbox("presN")
    .setPosition(320 + 220, 170 + 140 * 0)
    .setSize(80, 100)
    .setScrollSensitivity(1.1)
    .setValue(100000)
    .setCaptionLabel(" ")
    .lock()
    ;

  presLabel = cp5.addTextlabel("pres")
    .setText("Давление (Па)")
    .setPosition(320, 140 + 140 * 0)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myChart6 = cp5.addChart("altC")
    .setPosition(320, 170 + 140 * 1)
    .setSize(200, 100)
    .setRange(0, 30000)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart6.addDataSet("altC");
  myChart6.setData("altC", new float[100]);

  cp5.addNumberbox("altN")
    .setPosition(320 + 220, 170 + 140 * 1)
    .setSize(80, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  altLabel = cp5.addTextlabel("alt")
    .setText("Абсолютная высота (м)")
    .setPosition(320, 140 + 140 * 1)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myChart7 = cp5.addChart("altRelC")
    .setPosition(320, 170 + 140 * 2)
    .setSize(200, 100)
    .setRange(-50, 25000)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart7.addDataSet("altRelC");
  myChart7.setData("altRelC", new float[100]);

  cp5.addNumberbox("altRelN")
    .setPosition(320 + 220, 170 + 140 * 2)
    .setSize(80, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  altRelLabel = cp5.addTextlabel("altRel")
    .setText("Высота относительно запуска (м)")
    .setPosition(320, 140 + 140 * 2)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myChart10 = cp5.addChart("Bar2C")
    .setPosition(320, 170 + 140 * 3)
    .setSize(200, 100)
    .setRange(0, 120000)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart7.addDataSet("Bar2C");
  myChart7.setData("Bar2C", new float[100]);

  cp5.addNumberbox("Bar2N")
    .setPosition(320 + 220, 170 + 140 * 3)
    .setSize(80, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  bar2Label = cp5.addTextlabel("Bar2")
    .setText("Давление внутри камеры (Па)")
    .setPosition(320, 140 + 140 * 3)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;


  // ****************GPS****************


  GPSHead = cp5.addTextlabel("GPSHead")
    .setText("GPS")
    .setPosition(293, 727)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("BebasNeue-Regular.otf", 32))
    ;

  myChart8 = cp5.addChart("SputC")
    .setPosition(20, 765 + 40)
    .setSize(200, 100)
    .setRange(0, 20)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart8.addDataSet("SputC");
  myChart8.setData("SputC", new float[100]);

  cp5.addNumberbox("SputN")
    .setPosition(20 + 220, 765 + 40)
    .setSize(60, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  sputnikLabel = cp5.addTextlabel("Sput")
    .setText("Количество спутников")
    .setPosition(20,775)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myChart9 = cp5.addChart("GAltC")
    .setPosition(320, 765 + 40)
    .setSize(200, 100)
    .setRange(0, 25000)
    .setView(Chart.LINE) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
    //.setStrokeWeight(1.5)
    .setColorCaptionLabel(color(40))
    .setColorLabel(255)
    .setCaptionLabel(" ")
    ;
  myChart9.addDataSet("GAltC");
  myChart9.setData("GAltC", new float[100]);

  cp5.addNumberbox("GAltN")
    .setPosition(320 + 220, 765 + 40)
    .setSize(80, 100)
    .setScrollSensitivity(1.1)
    .setValue(404)
    .setCaptionLabel(" ")
    .lock()
    ;

  gpsAltLabel = cp5.addTextlabel("GAlt")
    .setText("Абсолютная GPS высота (м)")
    .setPosition(320, 765 + 10)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  longLabel = cp5.addTextlabel("GLong")
    .setText("Долгота: Not connected")
    .setPosition(20, 915)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;
  latiLabel = cp5.addTextlabel("GLati")
    .setText("Широта: Not connected")
    .setPosition(320, 915)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;
  // ****************CTRL****************

  CtrlHead = cp5.addTextlabel("CtrlHead")
    .setText("Управление контроллером")
    .setPosition(730, 93)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("BebasNeue-Regular.otf", 32))
    ;

  cp5.addToggle("BatHeat")
    .setPosition(820, 150)
    .setSize(70, 30)
    .setLabel("Подогрев \nаккумулятора")
    .lock()
    ;
  cp5.addToggle("CamHeat")
    .setPosition(940, 150)
    .setSize(70, 30)
    .setLabel("Подогрев \nкамеры")
    .lock()
    ;
  cp5.addToggle("EcoMode")
    .setPosition(940, 225)
    .setSize(70, 30)
    .setLabel("Eco Mode")
    ;
  cp5.addToggle("ManCtrl")
    .setPosition(700, 150)
    .setSize(70, 30)
    .setLabel("Ручное\nуправление")
    .setMode(ControlP5.SWITCH)
    .setValue(false)
    ;

  cp5.addBang("Upload_back")
    .setLabel("Перепрошить контроллер")
    .setPosition(700, 280)
    .setSize(70, 30)
    .setColorForeground(color(0, 45, 90))
    .setColorActive(color(0, 64, 128))
    ;
  cp5.addBang("Upload")
    .setPosition(700, 280)
    .setSize(70, 30)
    .setColorForeground(color(0, 45, 90))
    .setColorActive(color(0, 64, 128))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  cp5.addToggle("isUpload")
    .setPosition(820, 280)
    .setSize(70, 30)
    .setLabel(" ")
    .lock()
    ;
  cp5.addToggle("PhotoEn")
    .setPosition(940, 280)
    .setSize(70, 30)
    .setLabel("Камера")
    .lock()
    ;

  cp5.addToggle("TimeFill")
    .setPosition(700, 225)
    .setSize(190, 30)
    .lock()
    .setLabel(" ")
    ;
  Time = cp5.addTextlabel("Time")
    .setText("00:00:00")
    .setPosition(705, 226)
    //.setFont(createFont("Agency FB", 22));  // сделаем шрифт побольше
    .setFont(createFont("Tahoma", 20))
    ;
  Date = cp5.addTextlabel("Date")
    .setText("0/0/200")
    .setPosition(800, 226)
    //.setFont(createFont("Agency FB", 22));  // сделаем шрифт побольше
    .setFont(createFont("Tahoma", 20))
    ;

  cp5.addSlider("V3.3")
    .setPosition(700, 160 + 140 + 50)
    .setSize(20, 100)
    .setRange(0, 6)
    .lock()
    ;
  cp5.addSlider("V5")
    .setPosition(780, 160 + 140 + 50)
    .setSize(20, 100)
    .setRange(0, 6)
    .lock()
    ;
  cp5.addSlider("BatteryV")
    .setPosition(860, 160 + 140 + 50)
    .setSize(20, 100)
    .setRange(0, 15)
    .lock()
    .setLabel("Battery\nV")
    ;

  cp5.addSlider("amperage")
    .setPosition(940, 160 + 140 + 50)
    .setSize(20, 100)
    .setLabel("Потребление")
    .setRange(0, 6)
    .lock()
    ;


  // ****************SERIAL****************

  SerialHead = cp5.addTextlabel("SerialHead")
    .setText("Serial логирование")
    .setPosition(770, 458 + 50)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("BebasNeue-Regular.otf", 32))
    ;

  Monitor = cp5.addTextlabel("Monitor")
    .setText("Монитор порта:")
    .setPosition(670, 515 + 50)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  myTextarea = cp5.addTextarea("txt")
    .setPosition(670, 545 + 50)
    .setSize(320, 215)
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(200))
    .setColorBackground(color(0))
    .setColorForeground(color(255, 100));
  ;
  console = cp5.addConsole(myTextarea);//

  SendLog = cp5.addTextlabel("SendLog")
    .setText("Записать сообщение:")
    .setPosition(670, 545 + 215 + 10 + 50)
    .setColor(color(255))
    .setVisible(true)
    .setFont(createFont("Tahoma", 20))
    ;

  cp5.addTextfield("SendField")
    .setLabel(" ")
    .setPosition(670, 800 + 50)
    .setSize(200, 40)
    .setColorForeground(color(88, 93, 118))
    .setColorActive(color(0, 83, 166))
    .setAutoClear(false)
    ;

  cp5.addBang("clear")
    .setPosition(670 + 200 + 10, 800 + 50)
    .setSize(80, 40)
    .setColorForeground(color(0, 45, 90))
    .setColorActive(color(0, 64, 128))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  cp5.addBang("send")
    .setPosition(670 + 200 + 20 + 80, 800 + 50)
    .setSize(80, 40)
    .setColorForeground(color(0, 45, 90))
    .setColorActive(color(0, 64, 128))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;

  cp5.addToggle("isOnlineToggle")
    .setPosition(1000, 595)
    .setSize(70, 30)
    .setLabel("Статус \nсоединения")
    .lock()
    ;
  OnlineDelay = cp5.addTextlabel("delay")
    .setText("Delay: \nInf ms")
    .setPosition(1000, 665)
    .setColor(color(255))
    .setVisible(true)
    ;
};


public void clear() {
  cp5.get(Textfield.class, "SendField").clear();
}

public void send() {
  if (cp5.get(Textfield.class, "SendField").getText() != null) {
    textValue = cp5.get(Textfield.class, "SendField").getText();
    cp5.get(Textfield.class, "SendField").clear();
    serial.write("2," + textValue + ";");
  }
}

void ManCtrl(int val) {
  boolean flag = (val == 0);
  cp5.get(Toggle.class, "BatHeat").setLock(flag);
  cp5.get(Toggle.class, "CamHeat").setLock(flag);
  cp5.get(Toggle.class, "PhotoEn").setLock(flag);
  serial.write("0," + val + ";");
}

void BatHeat(int val) {
  if(cp5.get(Toggle.class, "ManCtrl").getValue() == 1) 
    serial.write("1," + int(cp5.get(Toggle.class, "CamHeat").getValue()) + "," + val + "," + int(cp5.get(Toggle.class, "EcoMode").getValue()) + "," + int(cp5.get(Toggle.class, "PhotoEn").getValue()) + ";");
}

void CamHeat(int val) {
  if(cp5.get(Toggle.class, "ManCtrl").getValue() == 1)
  serial.write("1," + val + "," + int(cp5.get(Toggle.class, "BatHeat").getValue()) + "," + int(cp5.get(Toggle.class, "EcoMode").getValue()) + "," + int(cp5.get(Toggle.class, "PhotoEn").getValue()) + ";");
}

void EcoMode(int val) {
  serial.write("1," + int(cp5.get(Toggle.class, "CamHeat").getValue()) + "," + int(cp5.get(Toggle.class, "BatHeat").getValue()) + "," + val + "," + int(cp5.get(Toggle.class, "PhotoEn").getValue()) + ";");
}

void PhotoEn(int val) {
  if(cp5.get(Toggle.class, "ManCtrl").getValue() == 1)
  serial.write("1," + int(cp5.get(Toggle.class, "CamHeat").getValue()) + "," + int(cp5.get(Toggle.class, "BatHeat").getValue()) + "," + int(cp5.get(Toggle.class, "EcoMode").getValue()) + "," + val + ";");
}

void Upload() {
  serial.write("3,1;");
}

int OnlineTimer;
int OnlineTimerDiff;

void checkOnline() {
  if(millis() - OnlineTimer >= 5000)
  {
    OnlineTimer = millis();
    if(serial != null) 
      serial.write("4, 1;");
  }
  if(millis() - OnlineTimerDiff > 10000)
  {
    cp5.get(Toggle.class, "isOnlineToggle").setValue(false);
  }
  OnlineDelay.setText("Delay: \n" + (OnlineTimerDiff - OnlineTimer) + " ms");
  
}

void com(int n) {
  portName = Serial.list()[n];  // запоминаем выбранный порт в portName
}

// кнопка открыть порт
void open() {
  if (portName != null && serial == null) {     // если выбран порт и сейчас он закрыт
    serial = new Serial(this, portName, speed); // открываем portName
  }
}

// кнопка закрыть порт
void close() {
  if (serial != null) { // если порт открыт
    serial.stop();      // закрываем portName
    serial = null;      // serial выключен
  }
}

void parsing() {
  // если порт открыт и в буфере что то есть
  if (serial != null && serial.available() > 0) {
    delay(40);
    String str = serial.readStringUntil('\n');  // читаем строку до \n и подрезаем
    if (str != null)
    {
      str = str.trim();
      println(str);
      String data[] = str.split(",");
      //if (skip) {
      //  skip = false;
      //  return;  // пропускаем первый пакет
      //}
      // разбиваем строку
      switch (int(data[0])) {  // свитч по ключу
      case 0:
        if (data.length == 4) {
          myChart.push("tempC", float(data[1]));
          cp5.get(Numberbox.class, "tempN").setValue(float(data[1]));
          myChart2.push("tempBatC", float(data[2]));
          cp5.get(Numberbox.class, "tempBatN").setValue(float(data[2]));
          myChart3.push("tempCamC", float(data[3]));
          cp5.get(Numberbox.class, "tempCamN").setValue(float(data[3]));
        }
        break;
      case 1:
        if (data.length == 5) {
          myChart4.push("tempBarC", float(data[1]));
          cp5.get(Numberbox.class, "tempBarN").setValue(float(data[1]));
          myChart5.push("presC", float(data[2]));
          cp5.get(Numberbox.class, "presN").setValue(float(data[2]));
          myChart6.push("altC", float(data[3]));
          cp5.get(Numberbox.class, "altN").setValue(float(data[3]));
          myChart7.push("altRelC", float(data[4]));
          cp5.get(Numberbox.class, "altRelN").setValue(float(data[4]));
        }
        break;
      case 2:
        if (data.length == 11) {
          String[] time = new String[3];
          String[] date = new String[3];
          for (int i = 0; i < 3; i++) {
            time[i] = data[i+1];
            date[i] = data[i+4];
          }
          String infoT = join(time, ":");
          String infoD = join(date, "/");
          Time.setText(infoT);
          Date.setText(infoD);
          String lati = (int(data[7]) != 0) ? data[7] : "Not connected";
          String longi = (int(data[8]) != 0) ? data[8] : "Not connected";
          latiLabel.setText("Широта: " + lati);
          longLabel.setText("Долгота: " + longi);
          myChart9.push("GAltC", float(data[9]));
          cp5.get(Numberbox.class, "GAltN").setValue(int(data[9]));
          myChart8.push("SputC", float(data[10]));
          cp5.get(Numberbox.class, "SputN").setValue(int(data[10]));
        }
        break;
      case 3:
        if (data.length == 5)
        {
          cp5.get(Slider.class, "BatteryV").setValue(float(data[1]));
          cp5.get(Slider.class, "V5").setValue(float(data[2]));
          cp5.get(Slider.class, "V3.3").setValue(float(data[3]));
          cp5.get(Slider.class, "amperage").setValue(float(data[4]));
          break;
        }
      case 4:
        if (data.length == 3)
        {
          cp5.get(Toggle.class, "BatHeat").setValue(int(data[1]));
          cp5.get(Toggle.class, "CamHeat").setValue(int(data[2]));
        }
        break;
      
      case 5:
        if (data.length == 2)
        {
          OnlineTimerDiff = millis();
          cp5.get(Toggle.class, "isOnlineToggle").setValue(true);
        }
        break;
      case 6:
        if (data.length == 2)
        {
          myChart10.push("Bar2C", float(data[1]));
          cp5.get(Numberbox.class, "Bar2N").setValue(int(data[1]));
        }
        break;
      }
    }
  }
}



void draw()
{
  parsing();
  checkOnline();
  background(0);      // Set background to white
  strokeWeight(1.5);
  fill(50);
  rect(0, 80, 650, 635);
  fill(30);
  rect(650, 80, 800, 1000);
  fill(90);
  rect(0, 740, 650, 875);
  fill(60);
  rect(650, 130, 800, 290 - 130 + 50);
  fill(80);
  rect(650, 290 + 50, 800, 290 - 135);
  fill(#2E2255);
  rect(0, 80, 650, 50);
  rect(0, 635 + 80, 650, 50);
  rect(650, 80, 1100 - 650, 50);
  rect(650, 445 + 50, 1100 - 650, 50);
  // print(mouseX);
  // print(" ");
  // println(mouseY);
}

void keyPressed() {
  switch(key) {
    case('1'):
    console.pause();
    break;
    case('2'):
    console.play();
    break;
    case('3'):
    console.setMax(13);
    break;
    case('4'):
    console.setMax(-1);
    break;
    case('5'):
    console.clear();
    break;
  }
}