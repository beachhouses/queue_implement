#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

struct Nasabah {
    string nomorAntrian;
    string waktuMasuk;
    string waktuKeluar;
    int serviceTime;
    string jenisLayanan;
};

// Ambil current date (DD/MM/YYYY)
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
       << setfill('0') << setw(2) << 1 + ltm->tm_mon << "/"
       << 1900 + ltm->tm_year;

    return ss.str();
}

// Tambah menit ke waktu string "HH:MM"
string tambahMenit(string waktu, int menitTambah) {
    int jam = stoi(waktu.substr(0, 2));
    int menit = stoi(waktu.substr(3, 2));

    menit += menitTambah;
    while (menit >= 60) {
        menit -= 60;
        jam++;
    }

    if (jam >= 15) jam = 15, menit = 0; // jam maksimal 15:00

    stringstream ss;
    ss << setfill('0') << setw(2) << jam << ":"
       << setfill('0') << setw(2) << menit;

    return ss.str();
}

string formatNomorAntrian(int nomor) {
    stringstream ss;
    ss << "B" << setfill('0') << setw(3) << nomor;
    return ss.str();
}

void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}

// Fungsi generate service time berdasarkan jenis layanan
int generateServiceTime(int jenisLayanan) {
    switch(jenisLayanan) {
        case 1: // Setor Tunai
            return 5 + rand() % 6;  // 5-10 menit
        case 2: // Tarik Tunai
            return 3 + rand() % 6;  // 3-8 menit
        case 3: // Pembukaan Rekening
            return 20 + rand() % 41; // 20-40 menit
        case 4: // Konsultasi
            return 10 + rand() % 16;  // 10-15 menit
        default:
            return 5 + rand() % 6;  // default 5-10 menit
    }
}

// Fungsi ambil nama layanan dari kode
string getNamaLayanan(int kode) {
    switch(kode) {
        case 1: return "Setor Tunai";
        case 2: return "Tarik Tunai";
        case 3: return "Pembukaan Rekening";
        case 4: return "Konsultasi";
        default: return "Layanan Tidak Diketahui";
    }
}

// Template function proses antrian
template <typename T>
void prosesAntrian(queue<T>& antrian, int jumlahNasabah, string tanggalHariIni) {
    float totalServiceTime = 0;
    int serviceTimeTercepat = INT_MAX;
    int serviceTimeTerlama = 0;
    string nasabahTercepat, nasabahTerlama;

    cout << "\nAntrian diproses..." << flush;
    for (int i = 0; i < 3; ++i) {
        cout << "." << flush;
        delay(500);
    }

    cout << "\n\n==================================================================================================\n";
    cout << " No Antri | Tanggal     | Waktu Masuk | Waktu Keluar | Jenis Layanan       | Service Time (menit)\n";
    cout << "==================================================================================================\n";

    queue<T> tempAntrian = antrian;

    while (!tempAntrian.empty()) {
        T n = tempAntrian.front();
        tempAntrian.pop();

        cout << setw(9) << n.nomorAntrian << " | "
             << setw(11) << tanggalHariIni << " | "
             << setw(11) << n.waktuMasuk << " | "
             << setw(12) << n.waktuKeluar << " | "
             << left << setw(19) << n.jenisLayanan << right << " | "
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

    cout << "==================================================================================================\n";
    cout << "Total Nasabah          : " << jumlahNasabah << " orang\n";
    cout << "Total Service Time     : " << totalServiceTime << " menit\n";
    cout << "Rata-rata Service Time : " << (totalServiceTime / jumlahNasabah) << " menit\n";
    cout << "Nasabah Tercepat       : " << nasabahTercepat << " (" << serviceTimeTercepat << " menit)\n";
    cout << "Nasabah Terlama        : " << nasabahTerlama << " (" << serviceTimeTerlama << " menit)\n";
    cout << "==================================================================================================\n";
    cout << "\nAntrian berhasil diproses!\n";
}

int main() {
    srand(time(0));
    queue<Nasabah> antrian;

    int jumlahNasabah;
    cout << "===================================================\n";
    cout << "            SELAMAT DATANG DI BANK HARMONI\n";
    cout << "           \"Selaras Dalam Setiap Transaksi\"\n";
    cout << "           SIMULASI SISTEM ANTRIAN PELAYANAN\n";
    cout << "===================================================\n\n";
    cout << "Jam Kerja Bank : 09:00 - 15:00\n";
    // Validasi input jumlah nasabah
    while (true) {
    cout << "Masukkan jumlah nasabah hari ini : ";
    cin >> jumlahNasabah;

    if (cin.fail() || jumlahNasabah <= 0) {
        cout << "Input tidak valid, silahkan masukkan angka yang valid!\n";
        cin.clear();
        cin.ignore(1000, '\n'); 
    } else {
        break;
        }
    }

    string tanggalHariIni = getCurrentDate();

    // generate jam awal
    int jam = 9; //mulai dari jam 09.00 (waktu minimum)
    int menit = 0;

    for (int i = 1; i <= jumlahNasabah; ++i) {
        Nasabah n;
        n.nomorAntrian = formatNomorAntrian(i);

        // Pilih jenis layanan secara acak (1-4)
        int jenisLayananKode = 1 + rand() % 4;
        n.jenisLayanan = getNamaLayanan(jenisLayananKode);

        // Hitung waktu masuk
        stringstream ss;
        ss << setfill('0') << setw(2) << jam << ":"
           << setfill('0') << setw(2) << menit;
        n.waktuMasuk = ss.str();

        // Generate waktu service sesuai jenis layanan
        n.serviceTime = generateServiceTime(jenisLayananKode);

        // Hitung waktu keluar
        n.waktuKeluar = tambahMenit(n.waktuMasuk, n.serviceTime);

        antrian.push(n);

        // update jam masuk berikutnya
        int jeda = 20 + rand() % 41; // 20-60 menit
        menit += jeda;
        while (menit >= 60) {
            menit -= 60;
            jam++;
        }
        if (jam >= 15) {
            jam = 15;
            menit = 0; // sampai jam 15.00 (waktu maksimum)
        }

        delay(200);
    }

    prosesAntrian<Nasabah>(antrian, jumlahNasabah, tanggalHariIni);

    return 0;
}
