#include <iostream>
#include <locale>
#include <string>
#include <cctype>

#include <fstream>
#include <sstream>
#include <vector>

#include <windows.h>
#include <conio.h>


#define PAUSE cout << "\n "; system("pause");
#define CLEAR system("cls");

using namespace std;

string submenu;
vector<string> menus;

bool acesso, programa = true;
string resposta;

enum Cores{
    Opcao = 2,
    Titulo = 10,
    Err = 4,
    Info = 8,
    Sucss = 10
};
struct Arquivo{
    int tamanho;
    string DIR;

    vector<string> dados;
    vector<string> *linhas;
};


struct _includes{

    //Transforma um texto em double
    double stod(string texto){

        double num;
        istringstream ss(texto);

        if(!(ss >> num)){
            return 0;
        }

        return num;
    }

    //Transforma um texto em inteiro
    int stoI(string texto)
    {
        int num;
        istringstream ss(texto);

        if(!(ss >> num)){
            return 0;
        }
        return num;
    }

    //Transforma um texto em char
    char *stoC(string texto)
    {

        char *newChar = new char[texto.size() + 1];

        for(int l = 0; l <= texto.size(); l++){
            newChar[l] = texto[l];
        }
        return newChar;
    }

    //Transforma letras/texto minuscula em maiuscula
    string to_upper(string texto)
    {
        string ntexto;

        for(int l = 0; l < texto.size(); l++){
            char letter = toupper(texto.at(l));
            ntexto += letter;
        }

        return ntexto;
    }

    //Transforma define a cor do texto do console
    void setCor(int cor = 15)
    {
        HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H, cor);
    }

    //Transforma define a cor de apenas um texto no console
    void textCor(string texto, int cor = 15)
    {
        setCor(cor);
        cout << texto;
        setCor();
    }

}In;

struct _fstream{

    //Separa as informações no documento
    char separador = ',';


    // Linhas - Retorna a quantidade de linhas que tem em um documento; Ignorando a CATEGORIA(primeira linha);
    int Linhas(string DIR)
    {

        int tamanho = 0;
        string linha, temp;

        fstream fin;
        fin.open(DIR, ios::in);

        if(fin.is_open()){
            while(fin >> temp){
                getline(fin, linha);
                tamanho++;
            }
        }
        fin.close();

        return tamanho - 1;
    }

    // Dados - Retorna todos os dados armazenados no documento
    Arquivo Dados(string DIR)
    {
        bool firstline;

        int LINHA = 0;
        string linha, palavra, temp;

        Arquivo FILE;
        FILE.DIR = DIR;
        FILE.tamanho = Linhas(DIR);
        FILE.linhas = new vector<string>[FILE.tamanho];

        fstream fin;
        fin.open(DIR, ios::in);

        while(fin >> temp){
            getline(fin, linha);
            stringstream ss(linha);

            if(!firstline){
                while(getline(ss, palavra, separador)){
                    FILE.dados.push_back(palavra);
                }
                firstline = true;
                continue;
            }

            while(getline(ss, palavra, separador)){
                FILE.linhas[LINHA].push_back(palavra);
            }
            LINHA++;
        }

        fin.close();
        return FILE;
    }

    // Adicionar - Adiciona uma nova informação no arquivo
    void Adicionar(Arquivo FILE)
    {

        int tamanho = Linhas(FILE.DIR);

        fstream fout;
        fout.open(FILE.DIR, ios::out | ios::app);
        fout.seekg(fout.end);

        fout << ", " << tamanho + 1;

        for(int COL = 1; COL < FILE.dados.size(); COL++){
            fout << "," << FILE.dados[COL];
        }

        fout << "\n";
        fout.close();

    }

    // Deletar - Remove uma informação/linha do arquivo
    void Deletar(string DIR, int LINHA)
    {

        Arquivo Cache;
        Cache.DIR = "files/cache.txt";

        string linha, palavra, temp;

        fstream fin, fout;
        fin.open(DIR, ios::in);
        fout.open(Cache.DIR, ios::out | ios::app);

        while(fin >> temp){

            Cache.dados.clear();
            getline(fin, linha);
            stringstream ss(linha);

            while(getline(ss, palavra, separador)){
                Cache.dados.push_back(palavra);
            }

            if(In.stoI(Cache.dados[0]) == -1){
                fout << ", -1";

                for(int col = 1; col < Cache.dados.size(); col++){
                    fout << "," << Cache.dados[col];
                }
                fout << "\n";
                fout.close();

                continue;
            }

            if(In.stoI(Cache.dados[0]) == LINHA){
                continue;
            }
            Adicionar(Cache);
        }

        fin.close();
        fout.close();

        char *novo = In.stoC(Cache.DIR), *antigo = In.stoC(DIR);
        remove(antigo);
        rename(novo, antigo);
    }

    // Editar - Procura a LINHA no documento e a COLUNA e armazena a nova informação
    void Editar(string DIR, int LINHA, int COLUNA, string VALOR)
    {

        Arquivo Cache;
        Cache.DIR = "files/cache.txt";

        string linha, palavra, temp;

        fstream fin, fout;
        fin.open(DIR, ios::in);
        fout.open(Cache.DIR, ios::out | ios::app);

        while(fin >> temp){

            Cache.dados.clear();
            getline(fin, linha);
            stringstream ss(linha);

            while(getline(ss, palavra, separador)){
                Cache.dados.push_back(palavra);
            }

            if(In.stoI(Cache.dados[0]) == -1){
                fout << ", -1";

                for(int col = 1; col < Cache.dados.size(); col++){
                    fout << "," << Cache.dados[col];
                }
                fout << "\n";
                fout.close();

                continue;
            }

            if(In.stoI(Cache.dados[0]) == LINHA){
                Cache.dados[COLUNA] = VALOR;
            }
            Adicionar(Cache);
        }

        fin.close();
        fout.close();

        char *novo = In.stoC(Cache.DIR), *antigo = In.stoC(DIR);
        remove(antigo);
        rename(novo, antigo);
    }

    // Filtro - Verificar se na LINHA tem tal informação(VALOR)
    bool Filtro(string DIR, int LINHA, string VALOR)
    {
        Arquivo FILE = Dados(DIR);

        for(int COLUNA = 0; COLUNA < FILE.dados.size(); COLUNA++){
            if(In.to_upper(VALOR) == In.to_upper(FILE.linhas[LINHA][COLUNA])){
                return true;
            }
        }
        return false;
    }

}Files;

struct _eventos{

    // Login - Verifica se tem tal login;
    bool Login(string login, string senha)
    {

        Arquivo Logins = Files.Dados("files/logins.txt");

        for(int LINHA = 0; LINHA < Logins.tamanho; LINHA++){
            if(Logins.linhas[LINHA][1] == login && Logins.linhas[LINHA][2] == senha){
                return true;
            }
        }
        return false;
    }

    // Censura - Cria um tipo de censura para variaveis tipo STRING
    string Censura(char sensu = '*')
    {
        string Senha;
        char Input;

        while(true){
            Input = getch();

            if(Input == 13){
                cout << endl;
                return Senha;

            }else if(Input == 8){
                if(Senha.size() != 0){
                    Senha.pop_back();
                    cout << "\b \b";
                }
                continue;
            }

            Senha.push_back(Input);
            cout << sensu;
        }
    }

    // Submenu - Administra o SUBMENU do Sistema
    void Submenu(string nome = "")
    {
        if(nome != ""){
            menus.push_back(submenu);
            submenu = nome;

            return;
        }

        int ultimo = (menus.size() - 1);
        submenu = menus[ultimo];
        menus.pop_back();
    }


    // Subtotal - Calcula o valor total de um documento, caso tenha uma CATEGORIA do tipo(VALOR & QUANTIDADE);
	void Subtotal(string DIR, string Texto, string FILTRO = "")
	{
        Arquivo FILE = Files.Dados(DIR);
        int SUBTOTAL = 0;

        for(int LINHA = 0; LINHA < FILE.tamanho; LINHA++){
            double QTD = 0, VALOR = 0;

            for(int COLUNA = 1; COLUNA < FILE.dados.size(); COLUNA++){
                if((FILTRO != "" ? Files.Filtro(DIR, LINHA, FILTRO) : true)){
                    if(FILE.dados[COLUNA] == "Valor"){
                        VALOR = In.stod(FILE.linhas[LINHA][COLUNA]);

                    }else if(FILE.dados[COLUNA] == "Quantidade"){
                        QTD = In.stod(FILE.linhas[LINHA][COLUNA]);

                    }
                }
            }

            QTD = QTD <= 0 ? 1 : QTD;
            SUBTOTAL += (VALOR*QTD);
        }

        In.textCor(" Subtotal das(os) "+Texto+": R$ ", Cores::Opcao);
        In.setCor(Cores::Info); cout << SUBTOTAL << endl; In.setCor();
	}

	// Listar - Lista todas as informação de um documento, sendo divido pela CATEGORIA;
    int Listar(string DIR, string TIPO = "", string FILTRO = "")
    {
    	int LISTOU = 0;
        Arquivo FILE = Files.Dados(DIR);

        for(int LINHA = 0; LINHA < FILE.tamanho; LINHA++){
            if((FILTRO != "" ? Files.Filtro(DIR, LINHA, FILTRO):true)){

                In.setCor(2);
                cout << (TIPO != "" ? " ["+TIPO+" Nº "+to_string(LINHA + 1)+"]\n":"");
                In.setCor();

                for(int COLUNA = 1; COLUNA < FILE.dados.size(); COLUNA++){
                   cout << "   - " << FILE.dados[COLUNA] <<
                   (FILE.dados[COLUNA] == "Valor" ? ": R$ " : ": ");
                   In.textCor(FILE.linhas[LINHA][COLUNA], Cores::Info);

                   cout << endl;
                   LISTOU++;
                }
                cout << endl;
            }
        }

        return LISTOU;
    }

    // Editar - Pegar as informação pelo usuário e envia para Files.Editar(), para fazer a editação no documento.
    void Editar(string DIR, int LINHA)
    {
        Arquivo FILE = Files.Dados(DIR);

        for(int col = 1; col < FILE.dados.size(); col++){
            In.textCor(" "+to_string(col)+". ", Cores::Opcao); cout << FILE.dados[col] << " // ";
            In.textCor(FILE.linhas[LINHA][col],Cores::Info); cout << endl;
        }

        int COLUNA;
        LINHA++;

        cout << endl << " Digite a opção que deseja mudar: ";
        In.setCor(Cores::Opcao); cin >> COLUNA; cin.sync(); In.setCor();

        if(COLUNA > 0 && COLUNA <= FILE.dados.size()){
            cout << " Digite o novo valor de " << FILE.dados[COLUNA] << ": ";
            In.setCor(Cores::Info); getline(cin, resposta); In.setCor();

            Files.Editar(DIR, LINHA, COLUNA, resposta);
            In.textCor("\n\n [SUCESS]: ",Cores::Sucss); cout << "Alteração feita com sucesso!";
            PAUSE
            return;

        }else{
            cout << endl;
            In.textCor(" [ERROR]: ", Cores::Err); cout << " Opção não encotrada";
        }
    }

    // Cadastrar - Pegar as informações pelo usuário e envia para Files.Adicionar(), para registrar uma nova informação.
    void Cadastrar(string DIR)
    {
        Arquivo FILE = Files.Dados(DIR);

        Arquivo Novo;
        Novo.DIR = DIR;
        Novo.dados.push_back(".");

        for(int COLUNA = 1; COLUNA < FILE.dados.size(); COLUNA++){
            In.textCor(" "+FILE.dados[COLUNA]+": ", Cores::Opcao);
            In.setCor(Cores::Info); getline(cin, resposta); In.setCor();

            Novo.dados.push_back(resposta);
        }


        Files.Adicionar(Novo);
        In.textCor("\n\n [SUCESS]: ",Cores::Sucss); cout << "Cadastro feito com sucesso!";
        PAUSE
    }

    // Remover - Pegar a LINHA pelo usuário e envia para Files.Remover(), para deletar a LINHA/COD no documento.
    void Remover(string DIR, int LINHA)
    {

        LINHA++;
        Files.Deletar(DIR, LINHA);

        In.textCor("\n\n [SUCESS]: ",Cores::Sucss); cout << "Removido com sucesso!";
        PAUSE

    }

}Event;


int main(){ setlocale(LC_ALL, "portuguese");

    string DIR; // Variavel DIR, para definir o diretorio de um menu quando for aberto

    //Sistema
    while(programa){

        //Sistema de Login
        while(!acesso){
            CLEAR
            In.textCor(" >> Login \n\n", Cores::Titulo);

            string login, senha;

            In.textCor(" Usuário: ", Cores::Opcao);
            getline(cin, login);

            In.textCor(" Senha: ", Cores::Opcao);
            senha = Event.Censura();
            acesso = Event.Login(login, senha);

            if(!acesso){
                cout << endl;
                In.textCor(" [ERROR]: ",Cores::Err); cout << "Acesso Negado. Tente novamente...";
                PAUSE
            
			}else{
				
				CLEAR
				In.setCor(Cores::Opcao);
				cout 
				<< "           +----------------------------------+" << endl
				<< "           ¦                                  ¦" << endl
				<< "           ¦    Seja Bem-vindo(a) ao Sistema  ¦" << endl
				<< "           ¦                                  ¦" << endl
				<< "           +----------------------------------+" << endl;
				In.setCor();
				
				In.textCor("                   [Versão 1.0.6.0 LTS]\n", Cores::Info);
				In.textCor("                  [Nem tudo são floress]\n", 12);
				
				cout << endl;
				PAUSE
			}
        }

        CLEAR
        In.textCor(" >> Menu Principal \n\n", Cores::Titulo);

        In.textCor(" 1. ",Cores::Opcao); cout << "Clientes" << endl;
        In.textCor(" 2. ",Cores::Opcao); cout << "Produtos" << endl;
        In.textCor(" 3. ",Cores::Opcao); cout << "Fornecedores" << endl;
        In.textCor(" 4. ",Cores::Opcao); cout << "Parceiros" << endl;
        In.textCor(" 5. ",Cores::Opcao); cout << "Ajuda" << endl;
        In.textCor(" 6. ",Cores::Opcao); cout << "Sobre" << endl;
        In.textCor(" 7. ",Cores::Opcao); cout << "Desconectar" << endl;


        cout << endl << " -> ";
        In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();

        switch(In.stoI(resposta)){

            //Clientes
            case 1:{

                Event.Submenu("clientes");
                DIR = "files/clientes.txt";

                while(submenu == "clientes"){
                    CLEAR
                    In.textCor(" >> Menu Principal > Clientes \n\n", Cores::Titulo);

                    In.textCor(" 1. ",Cores::Opcao); cout << "Pesquisar" << endl;
                    In.textCor(" 2. ",Cores::Opcao); cout << "Listar Todos" << endl;
                    In.textCor(" 3. ",Cores::Opcao); cout << "Cadastrar" << endl;
                    In.textCor(" 4. ",Cores::Opcao); cout << "Voltar" << endl;

                    cout << endl << " -> ";
                    In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();

                    switch(In.stoI(resposta)){
                        case 1:{

                            Event.Submenu("cliente/pesquisar");

                            while(submenu == "cliente/pesquisar"){
                                CLEAR
                                In.textCor(" >> Menu Principal > Clientes > Pesquisa \n\n", Cores::Titulo);
                                
                                Arquivo Clientes = Files.Dados(DIR);

                                cout << " > Digite o Nº do cliente(OBS: Digite -1 para voltar): ";
                                In.setCor(Cores::Info); cin >> resposta; cin.sync(); In.setCor();
                                
                                int LINHA = In.stoI(resposta);
                                
                                if(LINHA != -1){
                                    LINHA--;

                                    if(LINHA >= 0 && LINHA < Clientes.tamanho){
                                        cout << endl;
                                        Event.Listar(DIR,"Cliente",Clientes.linhas[LINHA][0]);

                                        In.textCor(" 1. ",Cores::Opcao); cout << "Editar" << endl;
                                        In.textCor(" 2. ",Cores::Opcao); cout << "Remover" << endl;
                                        In.textCor(" 3. ",Cores::Opcao); cout << "Histórico" << endl;
                                        In.textCor(" 4. ",Cores::Opcao); cout << "Registrar Compra" << endl;
                                        In.textCor(" 5. ",Cores::Opcao); cout << "Voltar" << endl;

                                        cout << endl << " -> ";
                                        In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();
                                        cout << endl;

                                        switch(In.stoI(resposta)){

                                            case 1:{
                                                CLEAR
                                                In.textCor(" >> Menu Principal > Clientes > Pesquisa > Editar \n\n", Cores::Titulo);
                                                Event.Editar(DIR, LINHA);

                                                continue;
                                            }

                                            case 2:{
                                                Event.Remover(DIR, LINHA);
                                                continue;
                                            }

                                            case 3:{
                                                In.textCor(" >> Histórico de Compras \n\n", Cores::Titulo);
                                                int LISTOU = Event.Listar("files/vendas.txt","Compra",Clientes.linhas[LINHA][1]);

                                                if(LISTOU == 0){
									                In.textCor(" [ERROR]: ",Cores::Err); cout << "Cliente não tem histórico de compras";
									                PAUSE
									                continue;
												}

												Event.Subtotal("files/vendas.txt","Compra",Clientes.linhas[LINHA][1]);
                                                PAUSE
                                                continue;
                                            }

                                            case 4:{

                                                Arquivo FILE = Files.Dados("files/vendas.txt");

                                                Arquivo NovaVenda;
                                                NovaVenda.DIR = "files/vendas.txt";
                                                NovaVenda.dados.push_back("debug");
                                                NovaVenda.dados.push_back(Clientes.linhas[LINHA][1]);


                                                In.textCor(" >> Menu Principal > Clientes > Registro de Venda \n\n", Cores::Titulo);

                                                In.textCor(" "+FILE.dados[1]+": ", Cores::Opcao);
                                                In.textCor(Clientes.linhas[LINHA][1], Cores::Info);
                                                cout << endl;

                                                for(int COLUNA = 2; COLUNA < FILE.dados.size(); COLUNA++){
                                                    In.textCor(" "+FILE.dados[COLUNA]+": ", Cores::Opcao);
                                                    In.setCor(Cores::Info); getline(cin, resposta); In.setCor();

                                                    NovaVenda.dados.push_back(resposta);
                                                }


                                                Files.Adicionar(NovaVenda);
                                                In.textCor("\n\n [SUCESS]: ",Cores::Sucss); cout << "Compra resgistrada feito com sucesso!";
                                                PAUSE

                                                continue;
                                            }

                                            case 5:{
                                                break;
                                            }

                                            default:{
                                                cout << endl;
                                                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
                                                PAUSE
                                                continue;
                                            }
                                        }


                                    }else{
                                        cout << endl;
                                        In.textCor(" [ERROR]: ",Cores::Err); cout <<"Opção não encontrada...";
                                        PAUSE

                                        continue;
                                    }
                                }
                                Event.Submenu();
                            }

                            continue;
                        }

                        case 2:{
                        	CLEAR
                    		In.textCor(" >> Menu Principal > Clientes > Lista \n\n", Cores::Titulo);

                        	Event.Listar(DIR,"Cliente");
                        	PAUSE
							continue;
						}

						case 3:{
							CLEAR
                    		In.textCor(" >> Menu Principal > Clientes > Novo \n\n", Cores::Titulo);


							Event.Cadastrar(DIR);
							continue;
						}

						case 4:{
							break;
						}

						default:{
							cout << endl;
			                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
			                PAUSE
							continue;
						}
                    }

                    Event.Submenu();
                }
                break;
            }

            //Produtos
			case 2:{
				Event.Submenu("produtos");
				DIR = "files/produtos.txt";

				while(submenu == "produtos"){
					CLEAR
                    In.textCor(" >> Menu Principal > Produtos \n\n", Cores::Titulo);

                    In.textCor(" 1. ",Cores::Opcao); cout << "Pesquisar" << endl;
                    In.textCor(" 2. ",Cores::Opcao); cout << "Listar Todos" << endl;
                    In.textCor(" 3. ",Cores::Opcao); cout << "Cadastrar" << endl;
                    In.textCor(" 4. ",Cores::Opcao); cout << "Produtos Vendidos" << endl;
                    In.textCor(" 5. ",Cores::Opcao); cout << "Produtos Anúnciados" << endl;
                    In.textCor(" 6. ",Cores::Opcao); cout << "Voltar" << endl;


                    cout << endl << " -> ";
                    In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();

                    switch(In.stoI(resposta)){

                    	case 1:{

                    		Event.Submenu("produtos/pesquisar");

                            while(submenu == "produtos/pesquisar"){
                                CLEAR
                                In.textCor(" >> Menu Principal > Produtos > Pesquisa \n\n", Cores::Titulo);

                                Arquivo Clientes = Files.Dados(DIR);

                                cout << " Digite o Nº do Produtos(OBS: Digite -1 para voltar): ";
                                In.setCor(Cores::Info); cin >> resposta; cin.sync(); In.setCor();

								int LINHA = In.stoI(resposta);
								
                                if(LINHA != -1){
                                    LINHA--;

                                    if(LINHA >= 0 && LINHA < Clientes.tamanho){
                                        cout << endl;
                                        Event.Listar(DIR,"Produto",Clientes.linhas[LINHA][0]);

                                        In.textCor(" 1. ",Cores::Opcao); cout << "Editar" << endl;
                                        In.textCor(" 2. ",Cores::Opcao); cout << "Remover" << endl;
                                        In.textCor(" 3. ",Cores::Opcao); cout << "Voltar" << endl;

                                        cout << endl << " -> ";
                                        In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();
                                        cout << endl;

                                        switch(In.stoI(resposta)){

                                            case 1:{
                                                CLEAR
                                                In.textCor(" >> Menu Principal > Produtos > Pesquisa > Editar \n\n", Cores::Titulo);
                                                Event.Editar(DIR, LINHA);

                                                continue;
                                            }

                                            case 2:{
                                                Event.Remover(DIR, LINHA);
                                                continue;
                                            }

                                            case 3:{
                                                break;
                                            }

                                            default:{
												cout << endl;
								                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
								                PAUSE
												continue;
											}

                                        }


                                    }else{
                                        cout << endl;
                                        In.textCor(" [ERROR]: ",Cores::Err); cout <<"Opção não encontrada...";
                                        PAUSE

                                        continue;
                                    }
                                }

                                Event.Submenu();
                            }

							continue;
						}

                    	case 2:{
                    		CLEAR
                   			In.textCor(" >> Menu Principal > Produtos > Lista \n\n", Cores::Titulo);
                   			Event.Listar(DIR,"Produto");

                   			PAUSE
							continue;
						}

                    	case 3:{

                    	    int LOOP = 0;

                    		CLEAR
                   			In.textCor(" >> Menu Principal > Produtos > Novo\n\n", Cores::Titulo);

                   			cout << " Quantos Produtos deseja cadastrar: ";
                   			In.setCor(Cores::Info); cin >> LOOP; cin.sync(); In.setCor();

                   			for(int i = LOOP; i > 0; i--){
                                CLEAR
                                In.textCor(" >> Menu Principal > Produtos > Novo\n\n", Cores::Titulo);
                                Event.Cadastrar(DIR);
                   			}

							continue;
						}

                    	case 4:{

                    		CLEAR
                  			In.textCor(" >> Menu Principal > Produtos > Vendidos \n\n", Cores::Titulo);
                  			Event.Listar("files/vendas.txt","Vendas");
                  			Event.Subtotal("files/vendas.txt","Produtos");


							PAUSE
							continue;
						}

                    	case 5:{

                    		CLEAR
                  			In.textCor(" >> Menu Principal > Produtos > Vendidos \n\n", Cores::Titulo);
                  			Event.Listar("files/anunciados.txt","Anúncio");
                  			Event.Subtotal("files/anunciados.txt","Anúncio");

                    		PAUSE
							continue;
						}

                    	case 6:{
							break;
						}

                    	default:{
							cout << endl;
			                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
			                PAUSE
							continue;
						}

					}

					Event.Submenu();
				}

				break;
			}

			//Fornecedores
			case 3:{
				Event.Submenu("forcedores");
				DIR = "files/forcedores.txt";

				while(submenu == "forcedores"){

					CLEAR
                    In.textCor(" >> Menu Principal > Fornecedores \n\n", Cores::Titulo);

					In.textCor(" 1. ",Cores::Opcao); cout << "Pesquisar" << endl;
                    In.textCor(" 2. ",Cores::Opcao); cout << "Listar Todos" << endl;
                    In.textCor(" 3. ",Cores::Opcao); cout << "Cadastrar" << endl;
                    In.textCor(" 4. ",Cores::Opcao); cout << "Voltar" << endl;

                    cout << endl << " -> ";
                    In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();

					switch(In.stoI(resposta)){

						case 1:{

							Event.Submenu("fornecedores/pesquisar");

                            while(submenu == "fornecedores/pesquisar"){
                                CLEAR
                                In.textCor(" >> Menu Principal > Fornecedores > Pesquisa \n\n", Cores::Titulo);
                                
                                Arquivo Clientes = Files.Dados(DIR);

                                cout << " Digite o Nº do Fornecedor(OBS: Digite -1 para voltar): ";
                                In.setCor(Cores::Info); cin >> resposta; cin.sync(); In.setCor();

								int LINHA = In.stoI(resposta);
								
                                if(LINHA != -1){
                                    LINHA--;

                                    if(LINHA >= 0 && LINHA < Clientes.tamanho){
                                        cout << endl;
                                        Event.Listar(DIR,"Fornecedor",Clientes.linhas[LINHA][0]);

                                        In.textCor(" 1. ",Cores::Opcao); cout << "Editar" << endl;
                                        In.textCor(" 2. ",Cores::Opcao); cout << "Remover" << endl;
                                        In.textCor(" 3. ",Cores::Opcao); cout << "Voltar" << endl;

                                        cout << endl << " -> ";
                                        In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();
                                        cout << endl;

                                        switch(In.stoI(resposta)){

                                            case 1:{
                                                CLEAR
                                                In.textCor(" >> Menu Principal > Fornecedores > Pesquisa > Editar \n\n", Cores::Titulo);
                                                Event.Editar(DIR, LINHA);

                                                continue;
                                            }

                                            case 2:{
                                                Event.Remover(DIR, LINHA);
                                                continue;
                                            }

                                            case 3:{
                                                break;
                                            }

                                            default:{
												cout << endl;
								                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
								                PAUSE
												continue;
											}

                                        }


                                    }else{
                                        cout << endl;
                                        In.textCor(" [ERROR]: ",Cores::Err); cout <<"Opção não encontrada...";
                                        PAUSE

                                        continue;
                                    }
                                }

                                Event.Submenu();
                            }

							continue;
						}

						case 2:{

							CLEAR
                    		In.textCor(" >> Menu Principal > Fornecedores > Lista \n\n", Cores::Titulo);
							Event.Listar(DIR,"Fornecedor");

							PAUSE
							continue;
						}

						case 3:{
							CLEAR
                   			In.textCor(" >> Menu Principal > Fornecedores > Novo \n\n", Cores::Titulo);
                   			Event.Cadastrar(DIR);

							continue;
						}

						case 4:{
							break;
						}

						default:{
                            cout << endl;
			                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
			                PAUSE
							continue;
						}
					}

					Event.Submenu();
				}


				break;
			}

			//Parceiros
			case 4:{
				Event.Submenu("parceiros");
				DIR = "files/parceiros.txt";

				while(submenu == "parceiros"){
					CLEAR
                    In.textCor(" >> Menu Principal > Parceiros \n\n", Cores::Titulo);


                    In.textCor(" 1. ",Cores::Opcao); cout << "Pesquisar" << endl;
                    In.textCor(" 2. ",Cores::Opcao); cout << "Listar Todos" << endl;
                    In.textCor(" 3. ",Cores::Opcao); cout << "Cadastrar" << endl;
                    In.textCor(" 4. ",Cores::Opcao); cout << "Voltar" << endl;

                    cout << endl << " -> ";
                    In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();

                    switch(In.stoI(resposta)){

                        case 1:{

                            Event.Submenu("parceiros/pesquisar");
                            while(submenu == "parceiros/pesquisar"){
                                CLEAR
                                In.textCor(" >> Menu Principal > Parceiros > Pesquisa \n\n", Cores::Titulo);

                                Arquivo Clientes = Files.Dados(DIR);

                                cout << " Digite o Nº do Parceiro(OBS: Digite -1 para voltar): ";
                                In.setCor(Cores::Info); cin >> resposta; cin.sync(); In.setCor();
                                
                                int LINHA = In.stoI(resposta);
                                
                                if(LINHA != -1){
                                    LINHA--;

                                    if(LINHA >= 0 && LINHA < Clientes.tamanho){
                                        cout << endl;
                                        Event.Listar(DIR,"Parceiro",Clientes.linhas[LINHA][0]);

                                        In.textCor(" 1. ",Cores::Opcao); cout << "Editar" << endl;
                                        In.textCor(" 2. ",Cores::Opcao); cout << "Remover" << endl;
                                        In.textCor(" 3. ",Cores::Opcao); cout << "Voltar" << endl;

                                        cout << endl << " -> ";
                                        In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();
                                        cout << endl;

                                        switch(In.stoI(resposta)){

                                            case 1:{
                                                CLEAR
                                                In.textCor(" >> Menu Principal > Fornecedores > Pesquisa > Editar \n\n", Cores::Titulo);
                                                Event.Editar(DIR, LINHA);

                                                continue;
                                            }

                                            case 2:{
                                                Event.Remover(DIR, LINHA);
                                                continue;
                                            }

                                            case 3:{
                                                break;
                                            }

                                            default:{
												cout << endl;
								                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
								                PAUSE
												continue;
											}

                                        }


                                    }else{
                                        cout << endl;
                                        In.textCor(" [ERROR]: ",Cores::Err); cout <<"Opção não encontrada...";
                                        PAUSE

                                        continue;
                                    }
                                }

                                Event.Submenu();
                                continue;
                            }
                        }

                        case 2:{
                            CLEAR
                    		In.textCor(" >> Menu Principal > Parceiros > Lista \n\n", Cores::Titulo);
							Event.Listar(DIR,"Parceiro");

							PAUSE
							continue;
                        }

                        case 3:{
                            CLEAR
                   			In.textCor(" >> Menu Principal > Parceiros > Novo \n\n", Cores::Titulo);
                   			Event.Cadastrar(DIR);

							continue;
                        }

                        case 4:{
                            break;
                        }

                        default:{
                            cout << endl;
			                In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
			                PAUSE
							continue;
                        }
                    }
                    Event.Submenu();
				}

				break;
			}

            //Ajuda
            case 5:{
                Event.Submenu("ajuda");

                while(submenu == "ajuda"){

                    CLEAR
                    In.textCor(" >> Menu Principal > Ajuda \n\n", Cores::Titulo);

                    In.textCor(" 1. ",Cores::Opcao); cout << "Menu Clientes" << endl;
                    In.textCor(" 2. ",Cores::Opcao); cout << "Menu Produtos" << endl;
                    In.textCor(" 3. ",Cores::Opcao); cout << "Menu Fornecedores" << endl;
                    In.textCor(" 4. ",Cores::Opcao); cout << "Menu Parceiros" << endl;
                    In.textCor(" 0. ",Cores::Opcao); cout << "Voltar" << endl;

                    cout << endl << " -> ";
                    In.setCor(Cores::Opcao); cin >> resposta; cin.sync(); In.setCor();
                    cout << endl;

                    switch(In.stoI(resposta)){


                        case 0:{
                            break;
                        }

                        case 1:{

                            In.textCor(" Cliente: ", Cores::Opcao);
                            cout << endl
                            << "  - Pesquisar(Pesquisa um cliente pelo Cod)" << endl
                            << "  - Listar Todos(Lista todos os clientes)" << endl
                            << "  - Cadastrar(Registra um novo cliente)" << endl;

                            PAUSE
                            continue;
                        }

                        case 2:{

                            In.textCor(" Produtos: ", Cores::Opcao);
                            cout << endl
                            << "  - Pesquisar(Pesquisa um cliente pelo Cod)" << endl
                            << "  - Listar Todos(Lista todos os clientes)" << endl
                            << "  - Cadastrar(Registra um novo cliente)" << endl
                            << "  - Produtos Vendidos(Exibe todas as vendas e subtotal)" << endl
                            << "  - Produtos Anúnciados(Exibe todos os anúncios e as informações)" << endl;

                            PAUSE;
                            continue;
                        }

                        case 3:{

                            In.textCor(" Fornecedores: ", Cores::Opcao);
                            cout << endl
                            << "  - Pesquisar(Pesquisa um fornecedor pelo Cod)" << endl
                            << "  - Listar Todos(Lista todos os fornecedores)" << endl
                            << "  - Cadastrar(Registra um novo fornecedor)" << endl;

                            PAUSE
                            continue;

                        }

                        case 4:{

                            In.textCor(" Parceiros: ", Cores::Opcao);
                            cout << endl
                            << "  - Pesquisar(Pesquisa um parceiro pelo Cod)" << endl
                            << "  - Listar Todos(Lista todos os Parceiros)" << endl
                            << "  - Cadastrar(Registra um novo parceiro)" << endl;

                            PAUSE
                            continue;
                        }

                    }

                    Event.Submenu();
                }

                break;
            }

            //Sobre
            case 6:{

                CLEAR
                In.textCor(" >> Menu Principal > Sobre \n\n", Cores::Titulo);

                cout << " GGHardware Software "; In.textCor("[versão 1.0.6.0 LTS]\n",Cores::Info);
                cout << " Sistema Desenvolvido por: "; In.textCor("Rogério, Rhian, Pablo & Gustavo\n\n",Cores::Info);

                cout << " Copyright © 2022 - TTTech Software Ltda - Todos os Direitos Reservados.\n";
                cout << " CNPJ: "; In.textCor("123.456.789/0001-12\n",Cores::Info);
                cout << " Contato Direto: "; In.textCor("TTTSoftwareOficial@gmail.com, (31) 9 93243-5436\n",Cores::Info);

                PAUSE
                break;
            }

            //Disconectar
            case 7:{
                acesso = false;
                break;
            }

            default:{
               cout << endl;
               In.textCor(" [ERROR]: ",Cores::Err); cout << "Opção não encontrada.";
               PAUSE
               break;
            }
        }

    }

}
