#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>
#include <ui_mainwindow.h>
#include <QSpinBox>
#include <QComboBox>

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
	�����, ������������� ������� �������� � �����-����� 

	\param[in, out] bwimage �����������, ������� ������ ���� ������������� �� �������� ������� � ��

	\code
	void mainwindow::toGray(QImage &bwimage) noexcept {
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
	�����, ����������� ���������� �����-������ ����������� 
	\param[in] bwimage �����-����� �����������, ������������ ��� ���������� ������� ������� �� �������  
	\param[in, out] bwimageCompressed ����� bwimage, ���������� ������� ��������� 
	\param[in] Xres ���������� �������������� �������� � �������������� Xres*Yres, ������� ������� ����� ��������� � ������� ��������
	\param[in] Yres ���������� ������������ �������� � �������������� Xres*Yres, ������� ������� ����� ��������� � ������� ��������
	\code
	void mainwindow::Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept {
		int widthStep = bwimage.width() / Xres;
		int heightStep = bwimage.height() / Yres;

		QImage newImage(Xres, Yres, QImage::Format_RGB32);

		for (int i = 0; i < Xres; ++i) {
			for (int j = 0; j < Yres; ++j) {
				int gray = 0;
				for (int x = 0; x < widthStep; ++x) {
					for (int y = 0; y < heightStep; ++y) {
						gray += bwimage.pixelColor(i * widthStep + x, j * heightStep + y).red();
					}
				}
				gray /= widthStep * heightStep;
				newImage.setPixelColor(i, j, QColor(gray, gray, gray));
			}
		}

		bwimageCompressed = newImage;
	}
	\endcode
 */
    void Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept;
/*!
	����� ��� ���������� ASCII ����
	\param[in] bwimageCompressed  �����������, ����������� ��� ���������� ASCII ��� 
	\param[out] gradient ������ ASCII ��������, ����������� �� �������� �������
	\param[in] Xres ���������� �������������� �������� � �������������� Xres*Yres
	\param[in] Yres ���������� ������������ �������� � �������������� Xres*Yres
	\return ������ ASCII ��������
	\code
	string mainwindow::Conversion(QImage &bwimageCompressed, const string& gradient, const int Xres, const int Yres) noexcept {
		string asciiArt;
		int width = bwimageCompressed.size().width();
		int height = bwimageCompressed.size().height();

		int widthStep = width / Xres;
		int heightStep = height / Yres;

		for (int y = 0; y < height; y += heightStep) {
			for (int x = 0; x < width; x += widthStep) {
				int grayValue = 0;
				for (int i = 0; i < heightStep; ++i) {
					for (int j = 0; j < widthStep; ++j) {
						if ((x + j < width) && (y + i < height)) {
							grayValue += bwimageCompressed.pixelColor(x + j, y + i).red();
						}
					}
				}
				grayValue /= widthStep * heightStep;
				asciiArt += getCorrespondence(grayValue, gradient);
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
/*!  �����, ��� ��������� ������ ��� ������� ���������� ����� �������
*	\param[in] inputFilePath  ���� �� ��������� �����������
	\param[in] width �������� ������ ����������� � ��������
	\param[in] height �������� ������ ����������� � ��������
	\param[in] gradient ������ ASCII ��������, ����������� ��� ��������� ASCII ����
	\param[in] outputPath ���� �� ��������� .txt �����
*   \code
*   void mainwindow::processCommandLineArguments(const QString& inputFilePath, int width, int height, const string& gradient, const QString& outputPath) noexcept {
		QImage image;
		if (!image.load(inputFilePath)) {
			qDebug() << "Failed to load the image: " << inputFilePath;
			return;
		}

		QImage bwimage(image);
		toGray(bwimage);

		QImage bwimageCompressed(image);
		Compression(bwimage, bwimageCompressed, width, height);

		string asciiArt = Conversion(bwimageCompressed, gradient, width, height);

		ofstream output(outputPath.toStdString());
		if (output) {
			output << asciiArt;
			output.close();
			qDebug() << "File saved to: " << outputPath;
		}
		else {
			qDebug() << "Error while attempting to save file to: " << outputPath;
		}
	}
*   \endcode
*/
	void processCommandLineArguments(const QString& inputFilePath, int width, int height, const string& gradient, const QString& outputPath) noexcept;

private slots:
/*!  ����� ��������� ������� ������� �� ������� "�������� �����������"
*
*   \code
*   void mainwindow::on_Choose_clicked(){
		path = QFileDialog::getOpenFileName(this, "�������� �����������", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
		if (path.isEmpty()) {
			ui.statusBar->showMessage("�������� ����������� ��������.");
			return;
		}

		QImage image;
		if (!image.load(path)) {
			ui.statusBar->showMessage("�� ������� ��������� �����������.");
			return;
		}

		QPixmap pixmap(path);
		QPixmap scaledPixmap = pixmap.scaled(256, 144, Qt::KeepAspectRatio, Qt::SmoothTransformation);

		ui.label_image->setPixmap(scaledPixmap);
		ui.label_image->setFixedSize(256, 144);

		ui.statusBar->showMessage("����������� �������!");

		int minWidth = 551;
		int minHeight = 350;

		this->setMinimumSize(minWidth, minHeight);

		std::filesystem::path file = path.toStdString();
		std::string stdfilename = file.stem().string();
		filename = QString::fromStdString(stdfilename);

		int imgWidth = image.width();
		int imgHeight = image.height();
		ui.xresSpinBox->setMaximum(imgHeight);
		ui.xresSpinBox->setMinimum(1);
		ui.yresSpinBox->setMaximum(imgWidth);
		ui.yresSpinBox->setMinimum(1);

		ui.xresSpinBox->setEnabled(true);
		ui.yresSpinBox->setEnabled(true);
		ui.Convert->setEnabled(!ui.combo_gradient->currentText().isEmpty() && !path.isEmpty());
		ui.combo_gradient->setEnabled(true);

		ui.label_width->setText(QString("�������� ������ ASCII-art � �������� (1-%1)").arg(imgWidth));
		ui.label_height->setText(QString("�������� ������ ASCII-art � ������� (1-%1)").arg(imgHeight));
	}
*   \endcode
*/
    void on_Choose_clicked(); 
 /*!  ����� ��������� ������� ������� �� ������� "�������������� � ���������"
*
*   \code
*   void mainwindow::on_Convert_clicked() {
		int Xres = ui.xresSpinBox->value(); 
		int Yres = ui.yresSpinBox->value();

		string outputPath;
		QString Qs = ui.combo_gradient->currentText();
		if ((Qs.startsWith('"') && Qs.endsWith('"')) || (Qs.startsWith('\'') && Qs.endsWith('\''))) {
			Qs = Qs.mid(1, Qs.length() - 2);
		}
		string gradient = Qs.toStdString();

		QImage image;
		image.load(path);

		QImage bwimage(image);
		toGray(bwimage); 

		QImage bwimageCompressed(image);
		Compression(bwimage, bwimageCompressed, Yres, Xres);

		string asciiArt = Conversion(bwimageCompressed, gradient, Yres, Xres);

		ui.statusBar->showMessage("����������� ���������!");

		QFileDialog q;
		QString qpath;
		qpath = q.getExistingDirectory();
		outputPath = qpath.toStdString();
		outputPath += "/" + filename.toStdString() + ".txt";

		ofstream output(outputPath);
		if (output) {
			output << asciiArt;
			output.close();
			ui.statusBar->showMessage("���� �������� �: " + QString::fromStdString(outputPath));
		}
		else {
			ui.statusBar->showMessage("������ ��� ���������� �����!");
		}
	}
*   \endcode
*/
    void on_Convert_clicked();
/*! ��� ������� ��������� ������� ������� � ���������� ������������ � ���������
 * ��������� ������ ����������� (��������, ������ �� �������� ��� ����������),
 * � ����������� �� ���������� ������������ �������, ����� ��� ������� ���� � ��������
 * ����� � ������������ �������� ���������� �����������.
*
*   \code
*   void mainwindow::updateConvertButtonState() {
		ui.Convert->setEnabled(!ui.combo_gradient->currentText().isEmpty() && !path.isEmpty());
	}
*   \endcode
*/
	void updateConvertButtonState();
private:
	QSpinBox* xresSpinBox; //!< ������ SpinBox, ���������� ������ ��������� ����������� � �������� 
	QSpinBox* yresSpinBox; //!< ������ SpinBox, ���������� ������ ��������� ����������� � ������� 
	QComboBox* comboGradient; //!< ������ ComboBox ��� ������ ��������� ��� ���������� ASCII ���� 
    QString path; //!< ����, ���������� � ���� ���������� ���� �� �����������, ������� �� ���������  
    QString filename; //!<  ������������ ����������� � ������� "*.jpg"
    Ui::mainwindow ui;
};
#endif // MAINWINDOW_H