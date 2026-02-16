# STM32_ModbusRTU_Slave_Module_Library
Hardware-independent ModbusRTU slave module library for STM32 MCUs . (STM32 F0 , F1 , F2 , F3 , F4 , F7 , G0 , G4 , L0 , L1 , L4 , L5 , C0 )
---
## Designer Note:
Bu proje bir Elektrik Mühendisliği öğrencisi olarak benim ilk ciddi STM32 projem. Bu kütüphaneyi yazarken çok fazla şey öğrendim ve elimden gelenin en iyisini yapmaya çalıştım. Bu projenin bana çok fazla katkısı oldu , umarım size de faydası olur.
---
## Note on Documentation Language:
[EN] The code descriptions and comment lines within the library are currently in Turkish. I will translate all comment lines to English in upcoming versions to make it global.
[TR] Kütüphane içerisinde bulunan açıklamalar ve yorum satırları şu anda sadece Türkçe'dir. Gelecek sürümde İngilizce versiyonunu paylaşacağım.
---


⚙️**KULLANIM İÇİN ÖRNEK PROJEYİDE İNCELEYEBİLİRSİNİZ**⚙️
⚙️**KULLANIM ÖNCESİNDE README MUTKALA OKUNMALIDIR**⚙️


## Table of Contents
* ⚙️[Function of the Library:](#function-of-the-library)⚙️
* ⚙️[Supported MCU Families:](#supported-mcu-families)⚙️
* ⚙️[Supported Modbus RTU Function Codes:](#supported-modbus-rtu-function-codes)⚙️
* ⚙️[Installation and CubeMX (.ioc) Settings:](#installation-and-cubemx-ioc-settings)⚙️
* ⚙️[Project Integration and Personalization:](#project-integration-and-personalization)⚙️
* ⚙️[Management of Connectivity Settings and Updating to Factory Settings:](#management-of-connectivity-settings-and-updating-to-factory-settings)⚙️

  
## Function of the Library: 
[TR] STM32 Modbus RTU Slave Modül Kütüphanesi, STM32 tabanlı cihazların USART çevre birimini kullanarak Modbus RTU protokolü ile "Slave" olarak haberleşmesine olanak sağlamak amacıyla geliştirilmiştir. Cihaz donanımına bir TTL to RS485 Converter modülü entegre edilerek, RS485 standardı üzerinden diğer endüstriyel cihazlarla Slave olarak (PLC, HMI vb.) sorunsuz iletişim kurması sağlanır. Kütüphane, piyasadaki Otomatik Flow Kontrollü ve klasik (DE/RE Pin tetiği kullanan) RS485 Converter'ların tamamıyla tam uyumlu çalışır.

### 🔄1. Kütüphanenin sunduğu temel yetenekler:
* Master cihazdan gelen tüm Read ve Write komutlarını arka planda tamamen otomatik olarak işler. Sizin ekstra bir kod yazmanıza gerek kalmadan; Coil (Output) açma/kapama, Discrete Input okuma veya Register değerlerini güncelleme işlemlerini kendi başına gerçekleştirir. Başarılı işlemlerde Master cihaza standartlara uygun cevap paketini, kural dışı durumlarda ise ilgili Exception mesajını anında geri gönderir.
* Cihazın fabrika çıkışı haberleşme ayarlarını (Slave ID, Baud Rate, Parity Bit) başlangıçta belirleyebilirsiniz.
* Projenizin ihtiyacına uygun şekilde **Coil**, **Discrete Input**, **Input Register** ve **Holding Register** veri haritalarının içeriklerini kolayca kişiselleştirebilirsiniz.
* Belirli Holding Register'ları Master cihaz üzerinden değiştirerek haberleşme ayarlarını kalıcı (Flash Memory) olarak güncelleyebilirsiniz.
* Bu ayarların unutulması ya da donanımsal olarak sıfırlanmak istenmesi durumunda, cihazı tek dokunuşla fabrika ayarlarına döndüren fiziksel bir sıfırlama butonu (Push Button) ve durum LED'i ekleyebilirsiniz.
  
**Üstelik tüm bu özelliklere sahip kütüphaneyi, sadece **üç** adet fonksiyonu belirtilen yerlere yerleştirerek hızlıca projenize dahil edebilirsiniz!**

### 🔄2. Performans Notu:
Receive ve Transmit işlemleri tamamen **DMA + Interrupt** tabanlı yapılandırılmıştır. Bu sayede CPU meşgul edilmez ve projenizin ana kod akışını engellemeden arka planda kusursuz bir şekilde çalışır.


## Supported MCU Families: 
`STM32F0`, `STM32F1`, `STM32F2`, `STM32F3`, `STM32F4`, `STM32F7`
`STM32G0`, `STM32G4`, `STM32L0`, `STM32L1`, `STM32L4`, `STM32L5`, `STM32C0`


## Supported Modbus RTU Function Codes: 
**Kütüphane, endüstriyel standartlardaki temel Modbus fonksiyonlarını eksiksiz destekler**
* `0x01` - Read Coils
* `0x02` - Read Discrete Inputs
* `0x03` - Read Holding Registers
* `0x04` - Read Input Registers
* `0x05` - Write Single Coil
* `0x06` - Write Single Register
* `0x0F` - Write Multiple Coils
* `0x10` - Write Multiple Registers
* `Error` - Exception Codes


## Installation and CubeMX (.ioc) Settings: 
Kütüphanenin mikrodenetleyiciyi meşgul etmeden çalışabilmesi için projeyi oluştururken STM32CubeMX (.ioc) arayüzü üzerinden aşağıdaki ayarların eksiksiz yapılması gerekmektedir:

### 🔄1. USART Donanım Ayarları
Modbus haberleşmesi için kullanacağınız UART birimini (Örn: USART1, USART2) `Asynchronous` modda aktif edip şu temel parametreleri ayarlayın:
🔹* **Baud Rate:** 9600 *(Not: Başlangıç için bu girilir, kod içinden değiştirilebilir.)*
🔹* **Word Length:** 8 Bits (Including Parity)
🔹* **Parity:** None
🔹* **Stop Bits:** 1
🔹* **NVIC Settings:** `USARTx global interrupt` kutucuğunu kesinlikle **işaretleyin** (Enable).
🔹* **DMA Settings:** `Add` butonuna basarak hem RX , TX kanallarını ekleyin. RX için `Mode:` **Circular**, TX için `Mode :` **Normal** olmalıdır. İkisi için de `Increment Address:` **Memory** olmalıdır.
 
### 🔄2. Kütüphaneyi Projeye Dahil Etme
**Bu yöntem kendi proje klosörleriniz ile kütüphane klasörlerinin aynı `Inc` , `Src` dosyasında durmasına sebebep olur. Olası karışıklıkların önüne geçmek için 3. maddede anlatılan yöntem tavsiye edilir**
1. GitHub deposundan indirdiğiniz `Inc` ve `Src` klasörlerinin içindeki tüm dosyaları kopyalayın.
2. Kendi STM32CubeIDE projenizin içindeki `Core/Inc` ve `Core/Src` klasörlerinin içine yapıştırın.

### 🔄3. Kütüphaneyi Projeye Dahil Etme (Klasör Düzeni) (Tavsiye Edilen)
Projelerinizin düzenli kalması ve sistem dosyalarıyla karışmaması için kütüphaneyi ayrı bir klasörde tutmanız şiddetle tavsiye edilir.

🔹**Adım 1: Dosyaları Taşıma**
1. Projenizin ana dizininde (Core ve Drivers klasörlerinin olduğu yerde) örneğin `Modbus_Library` adında yeni bir klasör oluşturun.
2. GitHub'dan indirdiğiniz `Inc` ve `Src` klasörlerini bu yeni klasörün içine kopyalayın.

🔹**Adım 2: IDE'ye Klasörü Tanıtma (Include Path)**
Derleyicinin başlık dosyalarını bulabilmesi için bu yeni klasörü IDE'ye tanıtmalısınız:
1. STM32CubeIDE'de sol taraftaki `Project Explorer` penceresinden proje isminize **sağ tıklayın** ve **Properties (Özellikler)** seçeneğine girin.
2. Sol menüden şu yolu izleyin: `C/C++ Build` -> `Settings`.
3. Açılan ekranda `Tool Settings` sekmesi altındaki `MCU GCC Compiler` -> **`Include paths`** menüsüne tıklayın.
4. Üstteki **Add... (Yeşil Artı)** simgesine tıklayın, `Workspace...` butonuna basın ve oluşturduğunuz `Modbus_Library/Inc` klasörünü seçip `Apply and Close` diyerek kaydedin.


## Project Integration and Personalization: 
Kütüphaneyi projenize ekledikten sonra, donanım pinlerinizi ve sensör verilerinizi sisteme tanıtmak için aşağıdaki dosyalar içerisinde belirtilen işlemleri yapmanız gerekmektedir:

### 🔄1. MB_USER_BOX.c İşlemleri
🔹1. `Src > MB_USER_BOX.c` dosyası içerisinde bulunan üç adet temel fonksiyonu, dosya içindeki yönergelere (yorum satırlarına) uygun olarak projenizdeki ilgili yerlere (`main.c` ve kesme dosyalarına) yerleştiriniz. 
🔹2. Fonksiyonların parametre kısımlarını (UART handle, BaudRate vb.) kendi projenize göre yönergelerde belirtildiği şekilde doldurunuz.
🔹3. 🚨Bu fonksiyonları çağırdığınız her dosyanın en üstüne `#include "MB_USER_BOX.h"` başlık dosyasını eklemeyi unutmayınız.

### 🔄2. MB_USER_REGISTERS_MAP_BOX.c İşlemleri
🔹1. **Coil ve Discrete Input Bağlantıları:** `Src > MB_USER_REGISTERS_MAP_BOX.c` dosyası içerisinde bulunan `Coil_Degisken Coil_Map[]` ve `Discrete_Input_Degisken Discrete_Input_Map[]` struct yapıları (dizileri) içerisine, yorum satırlarında belirtilen formata uygun olarak projenize özgü Coil (GPIO_Output) ve Discrete Input (GPIO_Input) pinlerinizi tanımlayabilirsiniz.
🔹2. **Register Kullanımı:** Aynı dosya içerisinde tanımlanan `MB_InputRegisters[]` ve `MB_HoldingRegisters[]` dizilerine, projenizin istediğiniz bir dosyasından erişim sağlayarak değişkenlerinizi, sensör değerlerini veya ADC verilerini atayabilirsiniz. 🚨 *(Erişim sağladığınız dosyalarda `#include "MB_USER_BOX.h"` başlık dosyasının eklendiğinden emin olunuz.)*

> 🚨 **ÇOK ÖNEMLİ NOT (Haberleşme Ayarlarının Korunması)**
> `MB_HoldingRegisters[]` dizisinin ilk 4 indeksi, cihazın haberleşme ayarlarının RAM'deki anlık karşılığını tutmaktadır:
> * `[0]` -> Slave ID
> * `[1]` -> Baud Rate (LOW)
> * `[2]` -> Baud Rate (HIGH)
> * `[3]` -> Parity Bit (`0x01`->EVEN, `0x02`->ODD, `0x03`->NONE)
> 
> **Kendi yazılımınız içerisinde bu 4 değere kesinlikle veri yazmaya veya değiştirmeye çalışmayınız!** Bu ayarlar sadece Master cihazdan gelen Modbus mesajları ile güncellenebilir. (Bir güncelleme geldiğinde kütüphane yeni ayarları otomatik olarak işlemcinin Flash hafızasına kaydeder ve cihaz açılıp kapatıldığında ayarlar güncellenir).


## Management of Connectivity Settings and Updating to Factory Settings: 
Kütüphane, cihazın haberleşme ayarlarını (Slave ID, Baud Rate, Parity) sahada dinamik olarak yönetebilmeniz için size iki güçlü yöntem sunar: Master üzerinden uzaktan güncelleme ve fiziksel buton ile donanımsal sıfırlama.

### 🔄1. Master Cihaz Üzerinden Uzaktan Güncelleme
Bir Master cihaz (PLC, HMI vb.) üzerinden cihazın haberleşme ayarlarını kalıcı olarak değiştirmek için `MB_HoldingRegisters[]` dizisinin ilk 4 adresine yazma işlemi (Function Code: `0x06` veya `0x10`) yapılmalıdır:

🔹* **Adres [0] (Slave ID):** Yeni cihaz adresi girilir (Standart: 1-247 arası).
🔹* **Adres [1] (Baud Rate LSB) & Adres [2] (Baud Rate MSB):** Yeni haberleşme hızı girilir (Örn: 9600, 115200). Değer 32-bit olduğu için iki adet 16-bit register'a bölünerek gönderilmelidir.
🔹* **Adres [3] (Parity):** Parite ayarı girilir (`0x01`->EVEN, `0x02`->ODD, `0x03`->NONE).

🔹**İşlem Akışı:** Master cihaz bu adreslere yeni değerleri başarıyla yazdığı anda, kütüphane bu verileri arka planda otomatik olarak MCU'nun Flash hafızasına kaydeder. Cihaz yeniden başlatıldığında (enerji kesilip verildiğinde) artık yeni ayarlarıyla iletişim kurmaya başlar. 🚨*(İstisnai bir durum olarak; Slave ID güncellendiğinde değişikliğin aktif olması için cihazın yeniden başlatılmasına gerek yoktur, cihaz anında yeni ID ile iletişime geçer. Diğer ayarlar için yukarıda belirtildiği gibidir. )*

### 🔄2. Donanımsal Sıfırlama (Factory Reset) Butonu
Sahada yaşanabilecek adres çakışmaları veya haberleşme ayarlarının unutulması gibi kritik durumlarda, cihaza ekleyeceğiniz fiziksel bir buton ile ayarları varsayılana döndürebilirsiniz. Bu işlem endüstriyel standartlarda, yanlışlıkla basılmalara karşı korumalı bir şekilde yönetir.

🔹**Çalışma Mantığı ve Güvenlik Koruması:**
1. **Basılı Tutma:** Reset butonuna basıldığında sistem önce **2 saniye** bekler (anlık dokunmaları veya elektriksel gürültüleri yok sayar).
2. **Onay Süreci:**  2 saniye sonunda buton hala basılıysa, fonksiyonda tanımladığınız **Durum LED'i 10 defa hızlıca yanıp sönerek** kullanıcıya işlemin başlayacağı uyarısını verir.
3. **İptal veya Onay:** Bu yanıp sönme süreci bitene kadar butonu basılı tutmaya devam etmeniz gerekir. Eğer süreç bitmeden butonu bırakırsanız işlem iptal edilir.
4. **Sıfırlama:** Yanıp sönme bittiğinde buton hala basılı durumdaysa, cihaz Flash hafızadaki mevcut tüm kullanıcı ayarlarını siler ve kütüphaneyi ilk başlattığınızdaki varsayılan ayarlara kalıcı olarak geri döner.

🔹**Donanım ve CubeMX Bağlantı Kuralları:**
Kütüphanenin bu özelliği doğru kontrol edebilmesi için donanım bağlantılarınızın ve STM32CubeMX ayarlarınızın şu şekilde olması şarttır:
* **Reset Butonu (GPIO_Input):** Butonun bir ucu MCU pinine, diğer ucu **VDD (3.3V / 5V)** hattına bağlanmalıdır. Pinin CubeMX üzerinden `Pull-down` ayarlı olması (veya dışarıdan Pull-down direnci atılması) gerekir. Butona basıldığında işlemciye HIGH (1) sinyali gitmelidir.
* **Durum LED'i (GPIO_Output):** LED'in Anot (+) bacağı MCU pinine, Katot (-) bacağı ise doğrudan **GND** hattına bağlanmalıdır. Sistem pini HIGH (1) yaptığında LED yanmalıdır.



