#include <iostream>
#include <fstream>
//#include <string>
#include <iomanip>
using namespace std;

struct Mahasiswa{
	char nama[50];
	char nim[11];
	char prodi[30];
};

int menu();

class Database{
	public:
		
		fstream database;
		
		void koneksi(fstream &data){
			data.open("database.txt", ios::out | ios::in | ios::binary);
			if(data.is_open()){
				cout << "Database ditemukan" << endl << endl;
			}else{
				cout << "Database tidak ditemukan, membuat database baru..." << endl << endl;
				data.close();
				data.open("database.txt", ios::trunc | ios::out | ios::in | ios::binary);
			}
		}
		
		void tambah_data(fstream &data, int posisi, Mahasiswa &inputMahasiswa){
			data.seekp((posisi - 1)*sizeof(Mahasiswa), ios::beg);
			data.write(reinterpret_cast<char*>(&inputMahasiswa), sizeof(Mahasiswa));
		}
		
		int ukuran_data(fstream &data){
			int start, end;
			data.seekg(0, ios::beg);
			start = data.tellg();
			data.seekg(0, ios::end);
			end = data.tellg();
			return (end-start)/sizeof(Mahasiswa);
		}
		
		Mahasiswa lihat_data(fstream &data, int posisi){
			Mahasiswa readMahasiswa;
			data.seekg((posisi - 1)*sizeof(Mahasiswa), ios::beg);
			data.read(reinterpret_cast<char*>(&readMahasiswa), sizeof(Mahasiswa));
			return readMahasiswa;
		}
	
};

class Crud{
	public:
		
		Database tambah ;
		Database lihat ;
		Database edit ;
		Database hapus ;
		
		
		void creat(fstream &data){
			cout << "---------- Tambah Data Mahasiswa ---------\n" << endl;
			Mahasiswa inputMahasiswa;
			int size = Crud::tambah.ukuran_data(data);
			cout << "Nama: ";
			cin.getline(inputMahasiswa.nama, 50);
			cout << "Nim: ";
			cin.getline(inputMahasiswa.nim, 11);
			cout << "Prodi: ";
			cin.getline(inputMahasiswa.prodi, 30);
			
			Crud::tambah.tambah_data(data, size+1, inputMahasiswa);
		}
		
		void read(fstream &data){
			
			int size = Crud::lihat.ukuran_data(data);
			Mahasiswa showMahasiswa;
			if(size == 0){
				cout << "============================================================================================================\n";
				cout << "|                Data Masih Kosong, Silahkan Tambah Data Terlebih Dahulu :)                                |\n";
				cout << "============================================================================================================\n";
			}else{
				cout << "============================================================================================================\n";
				cout << "|                                          Daftar Mahasiswa                                                |\n";
				cout << "============================================================================================================\n";
			 	cout << "| id  |                 Nama                    |       Nim         |            Prodi                     |\n";
			 	cout << "============================================================================================================\n";
				for(int i =1; i<=size; i++){
					showMahasiswa = Crud::lihat.lihat_data(data, i);
					cout << "| "<< setiosflags(ios::left) << setw(4) << i << "|";
					cout << " "<< setiosflags(ios::left) << setw(40) << showMahasiswa.nama << "|";
					cout << " "<< setiosflags(ios::left) << setw(18) << showMahasiswa.nim << "|";
					cout << " "<< setiosflags(ios::left) << setw(37) << showMahasiswa.prodi << "|";
					cout << "\n============================================================================================================\n";
				}	
			}
		}
		
		void update(fstream &data){
			int id;
			Mahasiswa updateMahasiswa;
			cout << "Pilih id, data yang ingin di edit: ";
			cin >> id;
			fflush(stdin);
			
			updateMahasiswa = Crud::edit.lihat_data(data, id);
			cout << "\nNama: " << updateMahasiswa.nama << endl;
			cout << "Nim: " << updateMahasiswa.nim << endl;
			cout << "Prodi: " << updateMahasiswa.prodi << endl;
			
			cout << "\nUbah Data" << endl;
			cout << "Nama: ";
			cin.getline(updateMahasiswa.nama, 50);
			cout << "Nim: ";
			cin.getline(updateMahasiswa.nim, 11);
			cout << "Prodi: ";
			cin.getline(updateMahasiswa.prodi, 50);
			
			Crud::edit.tambah_data(data, id, updateMahasiswa);
		}
		
		void del(fstream &data){
			int id, size, set_id=0;
			Mahasiswa copyMahasiswa;
			fstream dataCadangan;
			size = Crud::hapus.ukuran_data(data);
			
			cout << "Pilih id, data yang ingin di hapus: ";
			cin >> id;
			
			dataCadangan.open("cadangan.txt", ios::trunc | ios::out | ios::in | ios::binary);
			for(int i=1; i<=size; i++){
				copyMahasiswa = Crud::hapus.lihat_data(data, i);
				if(i == id){
					set_id++;
					continue;
				}else{
					Crud::hapus.tambah_data(dataCadangan, i - set_id, copyMahasiswa);
				}
			}
			
			size = Crud::hapus.ukuran_data(dataCadangan);
			data.close();
			data.open("database.txt", ios::trunc | ios::out | ios::binary);
			data.close();
			data.open("database.txt", ios::out | ios::in | ios::binary);
			
			for(int i=1; i<=size; i++){
				copyMahasiswa = Crud::hapus.lihat_data(dataCadangan, i);
				Crud::hapus.tambah_data(data, i, copyMahasiswa);
			}
		}	
};

int main(){
	Database data;
	Crud crud;
	data.koneksi(data.database);
	bool lup = false;
	lompat:
	int pilihan = menu();
	string kembali;
	
	enum option{CREATE = 1, READ, UPDATE, DELETE, FINISH};
	while(pilihan != FINISH){
		switch(pilihan){
			case CREATE:	
				crud.creat(data.database);
				cout << "\nBerhasil Menambah Data......" << endl;
				break;
			case READ:
				crud.read(data.database);
				break;
			case UPDATE:
				crud.read(data.database);
				crud.update(data.database);
				system("cls");
				cout << "\nBerhasil Mengubah Data......" << endl;
				crud.read(data.database);
				break;
			case DELETE:
				crud.read(data.database);
				crud.del(data.database);
				system("cls");
				cout << "\nBerhasil Menghapus Data......" << endl;
				crud.read(data.database);
				break;
			default:
				cout << "Pilihan Tidak Tersedia :)" << endl;
				break;
		}
			cout << endl;
		do{
			
			cout << "ketik 0 untuk kembali: " ;
			cin >> kembali;
			if(kembali == "0"){
				system("cls");
				goto lompat;
			}else{
				lup = true;
			}			
		}while(lup == true);
	}
	
	do{
		cout << "Keluar program ? (y/n): ";
		cin >> kembali;
		if(kembali == "n" || kembali == "N"){
			system("cls");
			goto lompat;
		}else if(kembali == "y" || kembali == "Y"){
			lup = false;
		}else{
			lup = true;
		}
	}while(lup == true);
	
	system("cls");
	cout << "Program Keluar ..." << endl;
}

int menu(){
	int input;
	string menu[5];
	menu[0] = "[1] Tambah Data Baru";
	menu[1] = "[2] Lihat Daftar Mahasiswa";
	menu[2] = "[3] Edit Data";
	menu[3] = "[4] Hapus Data";
	menu[4] = "[5] Keluar Program";
	cout << "---- DATA MAHASISWA ----" << endl;
	for(int i = 0; i < sizeof(menu)/sizeof(menu[0]); i++ ){
		cout << menu[i] << endl;
	}
	cout << "------------------------\n" << endl;
	cout << "Pilih menu: ";
	cin >> input;
	system("cls");
	fflush(stdin);
	return input;	
}
