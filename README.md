# Smart Bin with Real-Time Automated Waste Sorting

Proyek Capstone – Teknik Komputer 2022, Universitas Negeri Semarang    
Kelompok 15  
Anggota    : Muhammad Rafli Falam Athallah & Vacelnino Faiz Fadeli  
Pembimbing : Abdurrakhman Hamid Al-Azhari, M.T


## Deskripsi Proyek

Smart Bin ini dirancang untuk memilah **sampah anorganik** (plastik dan logam) secara otomatis menggunakan kombinasi teknologi **YOLOv5** untuk klasifikasi visual dan **Arduino** sebagai pengendali aktuator serta sensor. Sistem bekerja secara real-time dan responsif untuk meningkatkan efisiensi pengelolaan limbah skala kecil.


## Teknologi & Tools

| Komponen         | Teknologi                                                                 |
|------------------|---------------------------------------------------------------------------|
| Deteksi Objek    | YOLOv5 (custom-trained via Roboflow)                                      |
| Mikrokontroler   | Arduino UNO                                                               |
| Monitoring       | Sensor Ultrasonik HC-SR04                                                 |
| Mekanikal        | Servo Motor TowerPro SG90 (2 unit)                                        |
| Komunikasi       | Serial UART Python ↔ Arduino                                              |
| Tampilan         | OpenCV window + LED indikator (merah, kuning, hijau)                      |


## Dataset

Dataset yang digunakan adalah :

**[LIT Dataset - Roboflow Universe](https://universe.roboflow.com/litter-e1hys/lit-zyvzb)**

Deskripsi :
- Berisi ribuan gambar sampah (botol plastik, kaleng logam)
- Diunduh dan diproses menggunakan Roboflow
- Format : YOLOv5 annotation (.txt)
- Dua kelas dipilih : `PlasticBottle` dan `Metal Can`

### Preprocessing :
- Ukuran citra diubah ke 640 x 640 px
- Augmentasi : flipping, rotation, exposure
- Split data : 80% training, 20% validation
- Training dilakukan menggunakan **YOLOv5s** di **Google Colab**

**Hasil Training** :
- mAP@0.5 : **87.2%**
- Model disimpan sebagai `best.pt`

## Arsitektur Sistem

1. Kamera menangkap citra sampah.
2. YOLOv5 memproses dan mengklasifikasikan objek sebagai `PlasticBottle` atau `Metal Can`.
3. Label dikirim ke Arduino via serial.
4. Arduino menggerakkan servo ke kompartemen yang sesuai.
5. Sensor ultrasonik memantau kapasitas tempat sampah.
6. LED menyala sesuai status (hijau → kosong, kuning → setengah, merah → penuh).

## Cara Menjalankan Sistem
### Python (Deteksi & Komunikasi)

1. Pastikan kamera terpasang.
2. Jalankan `main.py` setelah mengganti path model dan port serial.
3. Tekan `q` untuk keluar dari tampilan kamera.

```bash
pip install torch opencv-python pyserial
python python-code/main.py
```
### Arduino

1. Upload `smartbin.ino` ke board Arduino UNO.
2. Pastikan koneksi ke sensor, LED, dan servo sudah sesuai wiring diagram.

### Pengembang
Muhammad Rafli Falam Athallah – [rafliathallah17@students.unnes.ac.id]

Program Studi Teknik Komputer, Universitas Negeri Semarang

### Lisensi
Repositori ini dibagikan untuk keperluan akademik dan pembelajaran.
Silakan gunakan dengan mencantumkan sumber.

### Tautan Penting
Dataset Roboflow : [https://universe.roboflow.com/litter-e1hys/lit-zyvzb]

YOLOv5 Documentation : [https://docs.ultralytics.com]
