#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <string>
#include <ui_mainwindow.h>

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
	Преобразует цветную картинку в черно-белую 

	\param[in, out] bwimage Изображение, которое должно быть преобразовано из цветного формата в чб

	\code
	void mainwindow::toGray(QImage &bwimage) noexcept {
		// Каждый пиксель изображения по трем его составляющим (Red, Green, Blue) трансформируется в серый пиксель 
		// Коэффициенты 0.2125, 0.7154, 0.0721 необходимы для трансформации цветного пикселя в чб "https://en.wikipedia.org/wiki/Grayscale"
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
	Уменьшает разрешение Черно-белого изображения 
	\param[in] bwimage Черно-белое изображение, используемое для считывания яроксти каждого чб пикселя  
	\param[in, out] bwimageCompressed Копия bwimage, разрешение которой сжимается 
	\param[in] Xres Количество горизонтальных пикселей в прямоугольнике Xres*Yres, яркость которых будет усреднена к единому значению
	\param[in] Yres Количество вертикальных пикселей в прямоугольнике Xres*Yres, яркость которых будет усреднена к единому значению
	\code
	void mainwindow::Compression(QImage& bwimage, QImage& bwimageCompressed, const int Xres, const int Yres) noexcept {
	// Пробегаемся по пикселям в прямоугольнике Xres*Yres, считываем их среднюю яркость, и всем этим пикселям устанавливаем это значение
	int gray = 0;
	for (int i = 0; i < bwimage.size().width() - Xres; i += Xres) {
		for (int j = 0; j < bwimage.size().height() - Yres; j += Yres) {
			for (int x = 0; x < Xres; x++) {
				for (int y = 0; y < Yres; y++) {
					gray += bwimage.pixelColor(i + x, j + y).red();
				}
			}
			gray /= Xres * Yres + 1; // Среднее арифметическое по яркости в прямоугольнике Xres*Yres
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
	Строит ASCII арт 
	\param[in] bwimageCompressed  Изображение, необходимое для построения ASCII арт 
	\param[out] gradient Массив ASCII символов, выстроенных по убыванию яркости
	\param[in] Xres Количество горизонтальных пикселей в прямоугольнике Xres*Yres
	\param[in] Yres Количество вертикальных пикселей в прямоугольнике Xres*Yres
	\return Массив ASCII символов
	\code
	string mainwindow::Conversion(QImage &bwimageCompressed, const string& gradient, const int Xres, const int Yres) noexcept {
	string asciiArt;
	for (int j = 0; j < bwimageCompressed.size().height(); j += Yres) {
		for (int i = 0; i < bwimageCompressed.size().width(); i += Xres) {
			asciiArt += getCorrespondence(bwimageCompressed.pixelColor(i, j).red(), gradient); //Использование функции getCorrespondence(int, string) для подбора соответствия яркости пикселя и подходящего ASCII символа 
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
private slots:
/*!  Метод обработки события нажатия на клавишу "Choose an image"
*
*   \code
*   void mainwindow::on_Choose_clicked(){
	path = QFileDialog::getOpenFileName(); // // Получаем абсолютный путь до изображения 
	ui.statusBar->showMessage("Изображение выбрано!");
	std::filesystem::path file = path.toStdString();
	std::string stdfilename = file.stem().string(); // Получаем имя файла в std::string
	filename = QString::fromStdString(stdfilename); // Переводим из std::string в Qstring
	}
*   \endcode
*/
    void on_Choose_clicked(); 
 /*!  Метод обработки события нажатия на клавишу "Convert and save"
*
*   \code
*   void mainwindow::on_Convert_clicked() {
	QString Qs = ui.lineEdit->text();
	int Xres = 30; // Устанавливаем по умолчанию значение Xres = 30
	if (Qs.toInt() < 30) Xres = Qs.toInt();
	int Yres = 2 * Xres; // Корректирующее соотношение сторон для вывода в блокнот
	string outputPath;
	string gradient = "@$8W9H4Z1l(r/!:. "; // Градиент символов, из которого будет строиться ASCII арт

	QImage image;
	image.load(path);
	
	QImage bwimage(image);
	toGray(bwimage); //Преобразуем цветное изображение в черно-белое 

	QImage bwimageCompressed(image);
	Compression(bwimage, bwimageCompressed, Xres, Yres); // Снижаем разрешение черной-белой фотографии

	string asciiArt = Conversion(bwimageCompressed, gradient, Xres, Yres); // Строим ASCII арт 
	int a = Qs.toInt();
	if (Qs.toInt() > 30) ui.statusBar->showMessage("Конвертация завершена! Было выставлено макс. допустимое значение (30)");
	else ui.statusBar->showMessage("Конвертация завершена!");
	QFileDialog q;
	QString qpath;
	qpath = q.getExistingDirectory(); // Получаем путь до директории куда будем сохранять ASCII арт 
	outputPath = qpath.toStdString();
	outputPath = outputPath + "/" + filename.toStdString() + ".txt";
	ofstream output(outputPath);
	if (output) output << asciiArt;
	}
*   \endcode
*/
    void on_Convert_clicked();
private:
    QString path; //!< Поле, содержащее в себе абсолютный путь до изображения, которое мы загружаем  
    QString filename; //!<  Наименование изображения в формате "xxx.jpg"
    Ui::mainwindow ui;
};
#endif // MAINWINDOW_H