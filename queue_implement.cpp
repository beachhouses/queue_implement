#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct Nasabah {
    string nomorAntrian;
    float waktuMasuk;
    float waktuKeluar;
    int serviceTime;
};

float tambahMenit(float waktu, int menit) {
    int jam = (int)waktu;
    int mnt = (waktu - jam) * 100;
    mnt += menit;
    while (mnt >= 60) {
        mnt -= 60;
        jam++;
    }
    return jam + mnt / 100.0;
}

void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}

string formatNomorAntrian(int nomor) {
    stringstream ss;
    ss << "B" << setfill('0') << setw(3) << nomor;
    return ss.str();
}

string formatWaktu(float waktu) {
    int jam = (int)waktu;
    int mnt = (waktu - jam) * 100;
    stringstream ss;
    ss << setfill('0') << setw(2) << jam << ":" << setw(2) << mnt;
    return ss.str();
}

int main() {
    queue<Nasabah> antrian;
    srand(time(0));

    int jumlahNasabah;

    cout << "===================================================\n";
    cout << "          SELAMAT DATANG DI BANK HARMONI                 \n";
    cout << "         \"Selaras Dalam Setiap Transaksi\"                \n";
    cout << "         SIMULASI SISTEM ANTRIAN PELAYANAN                 \n";
    cout << "===================================================\n\n";

    cout << "Masukkan jumlah nasabah hari ini : ";
    cin >> jumlahNasabah;

    // Waktu mulai jam 09:00
    float waktuSekarang = 9.00;

    vector<int> availableServiceTimes;
    for (int i = 5; i <= 15; ++i) {
        availableServiceTimes.push_back(i);
    }
    random_shuffle(availableServiceTimes.begin(), availableServiceTimes.end());

    for (int i = 1; i <= jumlahNasabah; i++) {
        Nasabah nasabah;
        nasabah.nomorAntrian = formatNomorAntrian(i);

        // Waktu masuk = waktuSekarang
        nasabah.waktuMasuk = waktuSekarang;

        // Service time random unik 5-15 menit
        if (!availableServiceTimes.empty()) {
            nasabah.serviceTime = availableServiceTimes.back();
            availableServiceTimes.pop_back();
        } else {
            // Jika jumlah nasabah lebih dari jumlah service time unik (11),
            // maka akan ada pengulangan. Untuk menghindari error, kita berikan nilai default.
            nasabah.serviceTime = 5 + rand() % 11;
        }

        nasabah.waktuKeluar = tambahMenit(nasabah.waktuMasuk, nasabah.serviceTime);

        // Tambah random 1-60 menit ke waktuSekarang untuk nasabah berikutnya
        int jeda = 1 + rand() % 60;
        waktuSekarang = tambahMenit(waktuSekarang, jeda);

        // Batas maksimal jam 15:00
        if (waktuSekarang >= 15.00) {
            waktuSekarang = 15.00;
        }

        antrian.push(nasabah);
    }

    cout << "\nAntrian diproses..." << flush;
    for (int i = 0; i < 3; ++i) {
        cout << "." << flush;
        delay(500);
    }
    cout << "\n\n====================================================================\n";
    cout << " No Antri | Waktu Masuk | Waktu Keluar | Service Time (menit)\n";
    cout << "====================================================================\n";

    float totalServiceTime = 0;
    int serviceTimeTercepat = INT_MAX;
    int serviceTimeTerlama = 0;
    string nasabahTercepat, nasabahTerlama;

    queue<Nasabah> tempAntrian = antrian;

    while (!tempAntrian.empty()) {
        Nasabah n = tempAntrian.front();
        tempAntrian.pop();

        cout << setw(9) << n.nomorAntrian << " | "
             << setw(11) << formatWaktu(n.waktuMasuk) << " | "
             << setw(12) << formatWaktu(n.waktuKeluar) << " | "
             << setw(19) << n.serviceTime << "\n";

        totalServiceTime += n.serviceTime;

        if (n.serviceTime < serviceTimeTercepat) {
            serviceTimeTercepat = n.serviceTime;
            nasabahTercepat = n.nomorAntrian;
        }

        if (n.serviceTime > serviceTimeTerlama) {
            serviceTimeTerlama = n.serviceTime;
            nasabahTerlama = n.nomorAntrian;
        }

        delay(300);
    }

    cout << "====================================================================\n";
    cout << "Total Nasabah           : " << jumlahNasabah << " orang\n";
    cout << "Total Service Time      : " << totalServiceTime << " menit\n";
    cout << "Rata-rata Service Time  : " << (totalServiceTime / jumlahNasabah) << " menit\n";
    cout << "Nasabah Tercepat        : " << nasabahTercepat << " (" << serviceTimeTercepat << " menit)\n";
    cout << "Nasabah Terlama         : " << nasabahTerlama << " (" << serviceTimeTerlama << " menit)\n";
    cout << "====================================================================\n";
    cout << "\nAntrian berhasil diproses!\n";

    return 0;
}