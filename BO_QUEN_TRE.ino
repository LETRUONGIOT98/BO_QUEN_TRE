#define analog_1 A0      // Định nghĩa chân analog A0 để đọc cảm biến trọng lực thứ nhất
#define analog_2 A1      // Định nghĩa chân analog A1 để đọc cảm biến trọng lực thứ hai
#define analog_3 A2      // Định nghĩa chân analog A2 để đọc cảm biến trọng lực thứ ba
#define trong_luc_4 A3      // Định nghĩa chân analog A3 để đọc cảm biến trọng lực thứ tư
int gioihan = 300;          // Giá trị giới hạn cho phép cho cảm biến, nếu vượt quá sẽ kích hoạt cảnh báo
#define coi 3               // Định nghĩa chân số 3 cho còi báo động
#define cua 5               // Định nghĩa chân số 5 để điều khiển cửa (mở/đóng)
long time;                  // Biến lưu trữ thời gian, dùng để kiểm tra thời gian chờ
#define nutnhan 2           // Định nghĩa chân số 2 là nút nhấn
int thoigian = 6000;        // Đặt khoảng thời gian chờ 6000ms (6 giây) để kiểm tra trạng thái cửa

void setup() {
  Serial.begin(9600);                   // Khởi tạo giao tiếp Serial với tốc độ 9600 baud
  //analogReference(EXTERNAL);            // Thiết lập tham chiếu điện áp cho chân analog từ chân AREF
  pinMode(12, INPUT_PULLUP);            // Thiết lập chân 12 là đầu vào với điện trở kéo lên (khi không nhấn là HIGH)
  pinMode(nutnhan, INPUT_PULLUP);       // Thiết lập chân nút nhấn là đầu vào với điện trở kéo lên
  pinMode(coi, OUTPUT);                 // Thiết lập chân còi là đầu ra để điều khiển báo động
  pinMode(cua, OUTPUT);                 // Thiết lập chân cửa là đầu ra để điều khiển cửa
}

void loop() {
  // Đọc giá trị analog từ cảm biến trọng lực thứ tư và gán vào biến val4
  float val4 = analogRead(trong_luc_4);
  float val1 = analogRead(analog_1);
  float val2 = analogRead(analog_2);
  float val3 = analogRead(analog_3);
  // In giá trị của cảm biến trọng lực thứ tư lên Serial để kiểm tra
  Serial.print(val4);
  Serial.print("   ");
  Serial.print(val3);
  Serial.print("   ");
  Serial.print(val2);
  Serial.print("   ");
  Serial.print(val1);
  Serial.println("   ");
  delay(100);                           // Dừng chương trình trong 100ms để tránh việc đọc quá nhanh

  // Đảo trạng thái của chân số 4 dựa trên trạng thái hiện tại của chân số 12
  digitalWrite(4, !digitalRead(12));

  // Kiểm tra xem chân số 12 có đang ở trạng thái HIGH hay không
  if (digitalRead(12) == 1) {
    // Nếu chân nút nhấn đang được nhấn (ở trạng thái LOW) và chân số 12 đang HIGH
    while (digitalRead(nutnhan) == 0 && digitalRead(12) == 1) {
        // Phát ra âm báo bằng cách bật tắt còi liên tục
        digitalWrite(coi, HIGH);
        delay(100);
        digitalWrite(coi, LOW);
        delay(100);
      }

    // Nếu giá trị cảm biến vượt quá giá trị giới hạn cho phép
    if ( val4 < 1023 || val1 < 1023 || val2 < 1023 || val3 < 1023){
      // Bật còi cảnh báo trong 500ms
      digitalWrite(coi, HIGH);
      delay(500);
      // Tắt còi cảnh báo trong 500ms
      digitalWrite(coi, LOW);
      delay(500);

      // Nếu thời gian hiện tại đã vượt quá thời gian chờ được đặt trước (thoigian)
      if (millis() - time >= thoigian) {
        // Mở cửa bằng cách đặt chân cửa ở trạng thái HIGH
        digitalWrite(cua, HIGH);
      }
      else {
        // Đóng cửa nếu thời gian chưa vượt qua thời gian chờ
        digitalWrite(cua, LOW);
      }
    }
    else {
      // Cập nhật lại thời gian để đếm ngược thời gian chờ
      time = millis();
      // Đảm bảo cửa ở trạng thái đóng nếu cảm biến chưa vượt ngưỡng giới hạn
      digitalWrite(cua, LOW);
    }
  }
  else {
    // Nếu chân số 12 ở trạng thái LOW, đảm bảo cửa đóng và còi tắt
    digitalWrite(cua, LOW);
    digitalWrite(coi, LOW);
    // Cập nhật lại thời gian để bắt đầu đếm ngược từ đầu
    time = millis();
  }
}
