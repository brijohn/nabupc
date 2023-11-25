
PROJECT=nabu_paddles

.PHONY: all clean $(PROJECT).pdf

all: $(PROJECT).pdf


$(PROJECT).pdf: $(PROJECT).tex
	latexmk -pdf -pdflatex="pdflatex -interactive=nonstopmode" -use-make $<

cleanall:
	latexmk -c

clean:
	latexmk -c
	@rm -rf $(PROJECT).pdf
