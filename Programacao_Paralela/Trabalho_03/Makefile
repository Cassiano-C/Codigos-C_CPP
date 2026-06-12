CXX = mpic++
CXXFLAGS = -O3 -Wall -std=c++11
LDFLAGS = -lm

TARGET = main
SOURCES = main.cpp
HEADERS = Jacobi.h Resposta.h

all: $(TARGET)

# Esta regra APENAS compila o programa de forma limpa
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Esta regra limpa os executáveis
clean:
	rm -f $(TARGET)

# Esta regra limpa os relatórios antigos se você quiser começar do zero
clear-data:
	rm -f *.csv

# 🔥 A MÁGICA PARA ADIANTAR O SEU LADO:
# O '@' antes do mpirun serve para não poluir o terminal com o comando bruto.
# Usamos o 'clear-data' como dependência para garantir que o CSV antigo seja limpo antes do novo teste.
run-all: clear-data $(TARGET)
	@echo "=== Iniciando testes automáticos para o TP3 ==="
	@echo "Rodando com 2 processos..."
	mpirun -np 2 ./$(TARGET)
	@echo "Rodando com 4 processos..."
	mpirun -np 4 ./$(TARGET)
	@echo "Rodando com 8 processos (usando as Threads de Hardware)..."
	# 🔥 ADICIONADO A FLAG --use-hwthread-cpus AQUI:
	mpirun --use-hwthread-cpus -np 8 ./$(TARGET)
	@echo "=== Testes concluídos! Gerando gráficos... ==="
	python3 gerar_graficos.py
	@echo "=== Gráficos gerados! Verifique os arquivos PNG ==="
	@echo "=== Todos os testes foram concluídos! Verifique resultados_jacobi.csv ==="

.PHONY: all clean clear-data run-all