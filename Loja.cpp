#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

struct Item {
    QString nome;
    double preco;
    int quantidade;
};

class LojaGUI : public QWidget {
    Q_OBJECT
private:
    QTableWidget *tabela;
    QList<Item> itens;

public:
    LojaGUI(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Sistema de Estoque");
        resize(500, 400);

        tabela = new QTableWidget(this);
        tabela->setColumnCount(3);
        tabela->setHorizontalHeaderLabels({"Nome", "Preço", "Quantidade"});

        QPushButton *btnAdd = new QPushButton("Adicionar Item");
        QPushButton *btnBaixa = new QPushButton("Dar Baixa");
        QPushButton *btnSalvar = new QPushButton("Salvar Dados");

        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(tabela);
        layout->addWidget(btnAdd);
        layout->addWidget(btnBaixa);
        layout->addWidget(btnSalvar);
        setLayout(layout);

        connect(btnAdd, &QPushButton::clicked, this, &LojaGUI::adicionarItem);
        connect(btnBaixa, &QPushButton::clicked, this, &LojaGUI::darBaixa);
        connect(btnSalvar, &QPushButton::clicked, this, &LojaGUI::salvarArquivo);
    }

public slots:
    void adicionarItem() {
        Item item;
        item.nome = QInputDialog::getText(this, "Adicionar", "Nome do item:");
        item.preco = QInputDialog::getDouble(this, "Adicionar", "Preço:");
        item.quantidade = QInputDialog::getInt(this, "Adicionar", "Quantidade:");
        itens.append(item);
        atualizarTabela();
    }

    void darBaixa() {
        int row = tabela->currentRow();
        if (row < 0) return;
        bool ok;
        int qtd = QInputDialog::getInt(this, "Dar baixa", "Quantidade vendida:", 1, 1, itens[row].quantidade, 1, &ok);
        if (ok) {
            itens[row].quantidade -= qtd;
            atualizarTabela();
        }
    }

    void salvarArquivo() {
        QFile file("estoque_gui.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const Item &i : itens)
                out << i.nome << ";" << i.preco << ";" << i.quantidade << "\n";
        }
        QMessageBox::information(this, "Salvo", "Estoque salvo com sucesso!");
    }

    void atualizarTabela() {
        tabela->setRowCount(itens.size());
        for (int i = 0; i < itens.size(); ++i) {
            tabela->setItem(i, 0, new QTableWidgetItem(itens[i].nome));
            tabela->setItem(i, 1, new QTableWidgetItem(QString::number(itens[i].preco)));
            tabela->setItem(i, 2, new QTableWidgetItem(QString::number(itens[i].quantidade)));
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LojaGUI loja;
    loja.show();
    return app.exec();
}

#include "main.moc"
