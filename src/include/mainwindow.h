#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>
#include <ui_mainwindow.h>

using namespace std;

/*! 
\brief Qt �����, ����������� ���� ���������.

�������� ������, ����������� ��������������� ������� ����������� � ASCII ���.
*/
class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = Q_NULLPTR);
/*!
	����������� ������� �������� � �����-����� 

	\param[in, out] bwimage �����������, ������� ������ ���� ������������� �� �������� ������� � ��

	\code
	void mainwindow::toGray(QImage &bwimage) noexcept {
		// ������ ������� ����������� �� ���� ��� ������������ (Red, Green, Blue) ���������������� � ����� ������� 
		// ������������ 0.2125, 0.7154, 0.0721 ���������� ��� ������������� �������� ������� � �� "https://en.wikipedia.org/wiki/Grayscale"
		for (int i = 0; i < bwimage.size().width(); i++) {
			for (int j = 0; j < bwimage.size().height(); j++) {
				int colored_to_gray = bwimage.pixelColor(i, j).red() * 0.2125 + bwimage.pixelColor(i, j).green() * 0.7154 + bwimage.pixelColor(i, j).blue() * 0.0721;
				bwimage.setPixelColor(i, j, QColor(colored_to_gray, colored_to_gray, colored_to_gray));
			}
		}
	}
    \endcode
 */
    void toGray(QImage& bwimage) noexcept;
/*!
	��������� ���������� �����-������ ����������� 
	\param[in] bwimage �����-����� �����������, ������������ ��� ���������� ������� ������� �� �������  
	\param[in, out] bwimageCompressed ����� bwimage, ���������� ������� ��������� 
	\param[in] Xres ���������� �������������� �������� � �������������� Xres*Yres, ������� ������� ����� ��������� � ������� ��������
	\param[in] Yres ���������� ������������ �������� � �������������� Xres*Yres, ������� ������� ����� ��������� � ������� ��������
	\code
	void mainwindow::Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept {
	// ����������� �� �������� � �������������� Xres*Yres, ��������� �� ������� �������, � ���� ���� �������� ������������� ��� ��������
	int gray = 0;
	for (int i = 0; i < bwimage.size().width() - Xres; i += Xres) {
		for (int j = 0; j < bwimage.size().height() - Yres; j += Yres) {
			for (int x = 0; x < Xres; x++) {
				for (int y = 0; y < Yres; y++) {
					gray += bwimage.pixelColor(i + x, j + y).red();
				}
			}
			gray /= Xres * Yres + 1; // ������� �������������� �� ������� � �������������� Xres*Yres
			for (int x = 0; x < Xres; x++) {
				for (int y = 0; y < Yres; y++) {
					bwimageCompressed.setPixelColor(i + x, j + y, QColor(gray, gray, gray));
				}
			}
		}
	}
	}	
	\endcode
 */
    void Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept;
/*!
	������ ASCII ��� 
	\param[in] bwimageCompressed  �����������, ����������� ��� ���������� ASCII ��� 
	\param[out] gradient ������ ASCII ��������, ����������� �� �������� �������
	\param[in] Xres ���������� �������������� �������� � �������������� Xres*Yres
	\param[in] Yres ���������� ������������ �������� � �������������� Xres*Yres
	\return ������ ASCII ��������
	\code
	string mainwindow::Conversion(QImage &bwimageCompressed, const string& gradient, const int Xres, const int Yres) noexcept {
	string asciiArt;
	for (int j = 0; j < bwimageCompressed.size().height(); j += Yres) {
		for (int i = 0; i < bwimageCompressed.size().width(); i += Xres) {
			asciiArt += getCorrespondence(bwimageCompressed.pixelColor(i, j).red(), gradient); //������������� ������� getCorrespondence(int, string) ��� ������� ������������ ������� ������� � ����������� ASCII ������� 
		}
		asciiArt += "\n";
	}
	return asciiArt;
	}
	\endcode
*/
    string Conversion(QImage& bwimageCompressed, const string& gradient, const int Xres, const int Yres) noexcept;
	/*!
		�����, �������������� ������� �� ������� � ����������� ��� ����� �������� ASCII ��������

		\param[in] brightness ������� ������� 
		\param[in] gradient ������ ASCII ��������, ����������� �� �������� �������
		\return ASCII ������, �������������� ������� �������� �������
		\code
		char mainwindow::getCorrespondence(int brightness, string gradient){
		return gradient[(int)((brightness / 255.f) * gradient.size())];
		}
		\endcode
	*/
    char getCorrespondence(int brightness, string gradient);
private slots:
/*!  ����� ��������� ������� ������� �� ������� "Choose an image"
*
*   \code
*   void mainwindow::on_Choose_clicked(){
	path = QFileDialog::getOpenFileName(); // // �������� ���������� ���� �� ����������� 
	ui.statusBar->showMessage("����������� �������!");
	std::filesystem::path file = path.toStdString();
	std::string stdfilename = file.stem().string(); // �������� ��� ����� � std::string
	filename = QString::fromStdString(stdfilename); // ��������� �� std::string � Qstring
	}
*   \endcode
*/
    void on_Choose_clicked(); 
 /*!  ����� ��������� ������� ������� �� ������� "Convert and save"
*
*   \code
*   void mainwindow::on_Convert_clicked() {
	QString Qs = ui.lineEdit->text();
	int Xres = 30; // ������������� �� ��������� �������� Xres = 30
	if (Qs.toInt() < 30) Xres = Qs.toInt();
	int Yres = 2 * Xres; // �������������� ����������� ������ ��� ������ � �������
	string outputPath;
	string gradient = "@$8W9H4Z1l(r/!:. "; // �������� ��������, �� �������� ����� ��������� ASCII ���

	QImage image;
	image.load(path);
	
	QImage bwimage(image);
	toGray(bwimage); //����������� ������� ����������� � �����-����� 

	QImage bwimageCompressed(image);
	Compression(bwimage, bwimageCompressed, Xres, Yres); // ������� ���������� ������-����� ����������

	string asciiArt = Conversion(bwimageCompressed, gradient, Xres, Yres); // ������ ASCII ��� 
	int a = Qs.toInt();
	if (Qs.toInt() > 30) ui.statusBar->showMessage("����������� ���������! ���� ���������� ����. ���������� �������� (30)");
	else ui.statusBar->showMessage("����������� ���������!");
	QFileDialog q;
	QString qpath;
	qpath = q.getExistingDirectory(); // �������� ���� �� ���������� ���� ����� ��������� ASCII ��� 
	outputPath = qpath.toStdString();
	outputPath = outputPath + "/" + filename.toStdString() + ".txt";
	ofstream output(outputPath);
	if (output) output << asciiArt;
	}
*   \endcode
*/
    void on_Convert_clicked();
private:
    QString path; //!< ����, ���������� � ���� ���������� ���� �� �����������, ������� �� ���������  
    QString filename; //!<  ������������ ����������� � ������� "xxx.jpg"
    Ui::mainwindow ui;
};
#endif // MAINWINDOW_H