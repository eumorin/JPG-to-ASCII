#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>
#include <ui_mainwindow.h>
#include <QSpinBox>
#include <QComboBox>

using namespace std;

/*! 
\brief Qt класс, описывающий окно приложеия.

Содержит методы, позволяющие сконвертировать цветное изображение в ASCII арт.
*/
class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = Q_NULLPTR);
/*!
	Метод, преобразующий цветную картинку в черно-белую 

	\param[in, out] bwimage Изображение, которое должно быть преобразовано из цветного формата в чб

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
	Метод, уменьшающий разрешение Черно-белого изображения 
	\param[in] bwimage Черно-белое изображение, используемое для считывания яроксти каждого чб пикселя  
	\param[in, out] bwimageCompressed Копия bwimage, разрешение которой сжимается 
	\param[in] Xres Количество горизонтальных пикселей в прямоугольнике Xres*Yres, яркость которых будет усреднена к единому значению
	\param[in] Yres Количество вертикальных пикселей в прямоугольнике Xres*Yres, яркость которых будет усреднена к единому значению
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
	Метод для построения ASCII арта
	\param[in] bwimageCompressed  Изображение, необходимое для построения ASCII арт 
	\param[out] gradient Массив ASCII символов, выстроенных по убыванию яркости
	\param[in] Xres Количество горизонтальных пикселей в прямоугольнике Xres*Yres
	\param[in] Yres Количество вертикальных пикселей в прямоугольнике Xres*Yres
	\return Массив ASCII символов
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
		Метод, сопоставляющий яркость чб пикселя с необходимым для этого значения ASCII символом

		\param[in] brightness Яркость пикселя 
		\param[in] gradient Массив ASCII символов, выстроенных по убыванию яркости
		\return ASCII символ, соответсвующий яркости входного символа
		\code
		char mainwindow::getCorrespondence(int brightness, string gradient){
			return gradient[(int)((brightness / 255.f) * gradient.size())];
		}
		\endcode
*/
    char getCorrespondence(int brightness, string gradient);
/*!  Метод, для обработки данных при запуске приложения через консоль
*	\param[in] inputFilePath  Путь до исходного изображения
	\param[in] width Выходная ширина изображения в столбцах
	\param[in] height Выходная высота изображения в строчках
	\param[in] gradient Массив ASCII символов, необходимый для отрисовки ASCII арта
	\param[in] outputPath Путь до выходного .txt файла
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
/*!  Метод обработки события нажатия на клавишу "Выберите изображение"
*
*   \code
*   void mainwindow::on_Choose_clicked(){
		path = QFileDialog::getOpenFileName(this, "Выберите изображение", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
		if (path.isEmpty()) {
			ui.statusBar->showMessage("Загрузка изображения отменена.");
			return;
		}

		QImage image;
		if (!image.load(path)) {
			ui.statusBar->showMessage("Не удалось загрузить изображение.");
			return;
		}

		QPixmap pixmap(path);
		QPixmap scaledPixmap = pixmap.scaled(256, 144, Qt::KeepAspectRatio, Qt::SmoothTransformation);

		ui.label_image->setPixmap(scaledPixmap);
		ui.label_image->setFixedSize(256, 144);

		ui.statusBar->showMessage("Изображение выбрано!");

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

		ui.label_width->setText(QString("Выберите ширину ASCII-art в столбцах (1-%1)").arg(imgWidth));
		ui.label_height->setText(QString("Выберите высоту ASCII-art в строках (1-%1)").arg(imgHeight));
	}
*   \endcode
*/
    void on_Choose_clicked(); 
 /*!  Метод обработки события нажатия на клавишу "Конвертировать и сохранить"
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

		ui.statusBar->showMessage("Конвертация завершена!");

		QFileDialog q;
		QString qpath;
		qpath = q.getExistingDirectory();
		outputPath = qpath.toStdString();
		outputPath += "/" + filename.toStdString() + ".txt";

		ofstream output(outputPath);
		if (output) {
			output << asciiArt;
			output.close();
			ui.statusBar->showMessage("Файл сохранен в: " + QString::fromStdString(outputPath));
		}
		else {
			ui.statusBar->showMessage("Ошибка при сохранении файла!");
		}
	}
*   \endcode
*/
    void on_Convert_clicked();
/*! Эта функция проверяет текущие условия в интерфейсе пользователя и обновляет
 * состояние кнопки конвертации (например, делает ее активной или неактивной),
 * в зависимости от выполнения определенных условий, таких как наличие пути к входному
 * файлу и корректность заданных параметров конвертации.
*
*   \code
*   void mainwindow::updateConvertButtonState() {
		ui.Convert->setEnabled(!ui.combo_gradient->currentText().isEmpty() && !path.isEmpty());
	}
*   \endcode
*/
	void updateConvertButtonState();
private:
	QSpinBox* xresSpinBox; //!< Объект SpinBox, отражающий ширину выходного изображения в столбцах 
	QSpinBox* yresSpinBox; //!< Объект SpinBox, отражающий высоту выходного изображения в строках 
	QComboBox* comboGradient; //!< Объект ComboBox для выбора градиента для построения ASCII арта 
    QString path; //!< Поле, содержащее в себе абсолютный путь до изображения, которое мы загружаем  
    QString filename; //!<  Наименование изображения в формате "*.jpg"
    Ui::mainwindow ui;
};
#endif // MAINWINDOW_H