1. Vì sao LED có "điện áp rơi" (forward voltage drop) cố định ~1.8–2.2V dù dòng điện qua nó thay đổi, và vì sao đây là lý do bắt buộc phải có điện trở hạn dòng mắc nối tiếp với LED?
 sao


Mỗi màu LED được làm từ vật liệu bán dẫn có một độ rộng vùng cấm (bandgap) riêng — đây là năng lượng tối thiểu electron cần có để nhảy qua vùng nghèo và phát ra photon ánh sáng. Năng lượng đó quyết định luôn bước sóng (màu sắc) ánh sáng phát ra (E = hc/λ). Vì bandgap là hằng số vật lý của vật liệu, nên điện áp cần thiết để "bơm" electron qua cũng gần như cố định — LED đỏ cần ít năng lượng hơn (~1.8–2.2V), LED xanh dương/trắng cần nhiều hơn (~3–3.4V) vì bandgap rộng hơn.

Vì sao đây là lý do bắt buộc phải có điện trở hạn dòng:

Khác với điện trở tuân theo định luật Ohm (V = I×R, quan hệ tuyến tính), đường đặc tuyến I-V của diode là hàm mũ: dưới ngưỡng điện áp rơi, gần như không có dòng chạy qua; nhưng chỉ cần vượt ngưỡng đó một chút, dòng điện tăng vọt rất nhanh theo hàm mũ.

Hệ quả: nếu cắm LED thẳng vào nguồn 3.3V mà không qua điện trở, bản thân LED không có cơ chế tự giới hạn dòng — chỉ cần nguồn nhỉnh hơn điện áp rơi một chút, dòng điện sẽ tăng không kiểm soát → LED nóng lên → điện trở nội giảm → dòng càng tăng thêm (hiệu ứng tự phá hủy, thermal runaway) → cháy LED gần như ngay lập tức, hoặc kéo dòng vượt quá mức GPIO chịu được (ESP32 giới hạn ~40mA/chân) → hỏng cả chân GPIO.

Điện trở mắc nối tiếp giải quyết việc này bằng cách gánh phần điện áp dư (V_trở = V_nguồn − V_LED) và ép dòng điện tuân theo quan hệ tuyến tính I = V_trở/R — tức là bạn chủ động chọn dòng điện mong muốn thay vì để LED tự quyết định (và nó sẽ luôn chọn "quá nhiều").

Ví von với backend : LED giống một service không có rate limit — nếu không chặn ở tầng trước (điện trở = reverse proxy giới hạn request/s), nó sẽ nhận request vô hạn cho đến khi sập.

2. Công thức dùng để tính R = 220Ω cho LED đỏ với nguồn 3.3V là gì, và nếu đổi sang LED xanh dương (Vf ~3.2V) thì điện trở cần đổi ra sao?
Vd tính LED đỏ Vf 2.2V
R = ( Vnguon - Vf)/ I
  = (3.3V - 2.2V )/ 0,04
  = 27,5

3. Nếu đổi sang LED xanh dương (Vf ≈ 3.2V) nhưng vẫn cấp từ GPIO 3.3V, công thức tính R sẽ gặp vấn đề gì?

R = (3.3 - 3.2) / 0.02 = 5Ω — rất nhỏ, nhưng không phải là "không cần điện trở".

Vấn đề là biên an toàn (margin) gần như bằng 0. Vf trên datasheet chỉ là giá trị trung bình — thực tế dao động theo sai số sản xuất (±0.1–0.2V) và nhiệt độ (Vf giảm khi LED nóng, càng nóng càng hút thêm dòng). Vì đường cong I-V là hàm mũ, chỉ cần Vf thực tế thấp hơn một chút là phần điện áp "dư" tăng gấp đôi/gấp ba → dòng điện tăng vọt → cháy LED, dù phép tính lý thuyết ra một con số rất nhỏ.

Cách làm đúng: vẫn gắn điện trở, chọn dư ra (ví dụ 100Ω thay vì 5Ω) để có margin, chấp nhận LED mờ hơn — hoặc đổi sang nguồn 5V để có đủ chênh lệch điện áp mà tính toán thoải mái hơn. Không bao giờ bỏ điện trở kể cả khi phép tính ra số gần 0.

4. Vì sao các mạch điều khiển LED công suất cao (như đèn LED strip, đèn xe) thường dùng nguồn dòng không đổi (constant-current driver) thay vì chỉ một điện trở nối tiếp đơn giản?

Nếu không dùng nguồn dòng không đổi, điện áp có thể tăng sụt, làm đèn mờ hoặc cháy đèn (đúng hướng, nhưng còn thiếu 2 lý do cốt lõi):

- Lãng phí năng lượng: điện trở "đốt" điện áp dư thành nhiệt (P = I²R). Ở dòng cao (350mA–1A+), drop 2V ở dòng 1A nghĩa là đốt 2W liên tục thành nhiệt — lãng phí điện, cần điện trở to và tản nhiệt tốt.
- Mất cân bằng dòng khi ghép nhiều LED: ghép nhiều LED song song/nối tiếp, chỉ cần 2 LED lệch Vf một chút là LED có Vf thấp hơn hút dòng nhiều hơn → nóng hơn → Vf giảm tiếp → hút dòng càng nhiều (thermal runaway tự khuếch đại) khiến 1 LED cháy trong khi LED bên cạnh vẫn mờ. Điện trở đơn giản không ngăn được việc này giữa nhiều nhánh.
- Điện áp nguồn không ổn định: ắc quy xe dao động 12–14.4V tùy máy nổ/tắt và mức sạc pin. Với điện trở cố định, dòng điện tỉ lệ thuận trực tiếp với biến động nguồn → đèn chớp nhòe hoặc dòng tăng vọt khi nguồn tăng cao.

Nguồn dòng không đổi giải quyết cả 3 vấn đề: chủ động đo và điều chỉnh để giữ dòng ổn định bất kể Vf lệch hay nguồn dao động, hiệu suất cao hơn cách "đốt" điện áp dư bằng điện trở.