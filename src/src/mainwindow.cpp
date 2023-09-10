#include <mainwindow.h>
#include <QFileDialog>
#include <QString>
#include <QLineEdit>
#include <QRegularExpression>
#include <QFile>
#include <QValidator>
#include <QImage>
#include <Qcolor>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	QRegularExpression exp("[1-9]{1}[0-9]{1}");
	ui.lineEdit->setValidator(new QRegularExpressionValidator(exp, this));
}

void mainwindow::on_Choose_clicked(){
	path = QFileDialog::getOpenFileName();
	ui.statusBar->showMessage("Изображение выбрано!");
	std::filesystem::path file = path.toStdString();
	std::string stdfilename = file.stem().string();
	filename = QString::fromStdString(stdfilename);
}

char mainwindow::getCorrespondence(int brightness, string gradient){
	return gradient[(int)((brightness / 255.f) * gradient.size())];
}

void mainwindow::toGray(QImage &bwimage) noexcept {
	for (int i = 0; i < bwimage.size().width(); i++) {
		for (int j = 0; j < bwimage.size().height(); j++) {
			int colored_to_gray = bwimage.pixelColor(i, j).red() * 0.2125 + bwimage.pixelColor(i, j).green() * 0.7154 + bwimage.pixelColor(i, j).blue() * 0.0721;
			bwimage.setPixelColor(i, j, QColor(colored_to_gray, colored_to_gray, colored_to_gray));
		}
	}
}

void mainwindow::Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept {
	int gray = 0;
	for (int i = 0; i < bwimage.size().width() - Xres; i += Xres) {
		for (int j = 0; j < bwimage.size().height() - Yres; j += Yres) {
			for (int x = 0; x < Xres; x++) {
				for (int y = 0; y < Yres; y++) {
					gray += bwimage.pixelColor(i + x, j + y).red();
				}
			}
			gray /= Xres * Yres + 1;
			for (int x = 0; x < Xres; x++) {
				for (int y = 0; y < Yres; y++) {
					bwimageCompressed.setPixelColor(i + x, j + y, QColor(gray, gray, gray));
				}
			}
		}
	}
}

string mainwindow::Conversion(QImage &bwimageCompressed, const string& gradient, const int Xres, const int Yres) noexcept {
	string asciiArt;
	for (int j = 0; j < bwimageCompressed.size().height(); j += Yres) {
		for (int i = 0; i < bwimageCompressed.size().width(); i += Xres) {
			asciiArt += getCorrespondence(bwimageCompressed.pixelColor(i, j).red(), gradient);
		}
		asciiArt += "\n";
	}
	return asciiArt;
}

void mainwindow::on_Convert_clicked() {
	QString Qs = ui.lineEdit->text();
	int Xres = 30;
	if (Qs.toInt() < 30) Xres = Qs.toInt();
	int Yres = 2 * Xres;
	string outputPath;
	string gradient = "@$8W9H4Z1l(r/!:. ";

	QImage image;
	image.load(path);
	
	QImage bwimage(image);
	toGray(bwimage);

	QImage bwimageCompressed(image);
	Compression(bwimage, bwimageCompressed, Xres, Yres);

	string asciiArt = Conversion(bwimageCompressed, gradient, Xres, Yres);
	int a = Qs.toInt();
	if (Qs.toInt() > 30) ui.statusBar->showMessage("Конвертация завершена! Было выставлено макс. допустимое значение (30)");
	else ui.statusBar->showMessage("Конвертация завершена!");
	QFileDialog q;
	QString qpath;
	qpath = q.getExistingDirectory();
	outputPath = qpath.toStdString();
	outputPath = outputPath + "/" + filename.toStdString() + ".txt";
	ofstream output(outputPath);
	if (output) output << asciiArt;
}