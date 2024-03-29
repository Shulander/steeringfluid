%%=============================================================================
%% mdtufsm.cls - Classe para formata��o de MDT da UFSM
%%
%% This program is free software; you can redistribute it and/or modify
%% it under the terms of the GNU General Public License as published by
%% the Free Software Foundation; either version 2 of the License, or
%% (at your option) any later version.
%%
%% This program is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%% GNU General Public License for more details.
%%
%% You should have received a copy of the GNU General Public License
%% along with this program; if not, write to the Free Software
%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
%%
%% Esta classe � uma adapta��o de iiufrgs.cls, contida pacote iiufrgs-4.3.1, 
%% desenvolvido pelo UFRGS TeX Users Group (http://www.inf.ufrgs.br/utug).
%%
%% A formata��o de documentos segue as recomenda��es contidas na
%% Estrutura e Apresenta��o de Monografias, Disserta��es e Teses - MDT, 
%% 6a. edi��o, 2006, dispon�vel em: http://coralx.ufsm.br/prpgp/pdf/mdt.pdf
%% 
%%=============================================================================

%%=============================================================================
%% Identifica��o
%%=============================================================================
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{mdtufsm}[2006/11/26 v1.0.0 Formatacao de monografias, dissertacoes e teses da UFSM]

%%=============================================================================
%% Declara��o de op��es
%%=============================================================================
\DeclareOption{tese}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{\tesespecificinfo}
    \def\doctypename{Tese de Doutorado}
    \def\doctypearticle{a}
  }
}
\DeclareOption{espec}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
      Monografia apresentada ao \@altcourse{} da \ufsm{} 
      (UFSM, RS), como requisito parcial para a 
      obten{\c{c}}{\~a}o do grau de\\[6pt]
      \textbf{\@degree}
    }

    \def\doctypename{Monografia de Especializa��o}
    \def\doctypearticle{a}
  }
}
\DeclareOption{tg}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
      Trabalho de Gradua{\c{c}}{\~a}o apresentado ao \@altcourse{} da \ufsm{}
      (UFSM, RS), como requisito parcial para a
      obten{\c{c}}{\~a}o do grau de\\[6pt]
      \textbf{\@degree}
    }
    \def\doctypename{Trabalho de Gradua��o}
    \def\doctypenameIngles{Graduation Work}
    \def\doctypearticle{o}
  }

}
\DeclareOption{diss}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
      Disserta{\c{c}}{\~a}o apresentada ao \@altcourse{} da \ufsm{} 
      (UFSM, RS), como requisito parcial para a 
      obten{\c{c}}{\~a}o do grau de\\[6pt]
      \textbf{\@degree}
    }
    \def\doctypename{Disserta��o de Mestrado}
    \def\doctypearticle{a}
    \def\doctypenameIngles{Master's Dissertation}
  }
}
\DeclareOption{english}{\@englishtrue\OptionNotUsed}\newif\if@english
% TODO: incluir opcao twoside
\DeclareOption{oneside}{\AtEndOfClass{\@twosidefalse}\OptionNotUsed}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ProcessOptions\relax


%% Disserta��o apresentada ao Curso de Mestrado do Programa de
%% P�s-Gradua��o em Engenharia de Produ��o, �rea de Concentra��o em
%% Tecnologia da Informa��o, da Universidade Federal de Santa Maria (UFSM, RS),
%% como requisito parcial para obten��o do grau de
%% Mestre em Engenharia de Produ��o.


%% Disserta��o apresentada ao Curso de Mestrado em Computa��o do Programa de
%% P�s-Gradua��o em Inform�tica da Universidade Federal de Santa Maria (UFSM, RS),
%% como requisito parcial para obten��o do grau de
%% Mestre em Computa��o.

%% Monografia apresentada ao Curso de Especializa��o em Sistemas
%% de Computa��o para a Web do Programa de P�s-Gradua��o em Inform�tica
%% da Universidade Federal de Santa Maria (UFSM, RS),
%% como requisito parcial para obten��o do grau de
%% Especialista em Sistemas de Computa��o para a Web.


%% Trabalho de Gradua��o apresentado ao Curso de Ci�ncia da Computa��o -- Bacharelado da Universidade Federal de Santa Maria (UFSM, RS), como requisito parcial para obten��o do grau de Bacharel em Ci�ncia da Computa��o.


%%=============================================================================
%% Comandos e defini��es gerais
%%=============================================================================
\def\ufsm{Universidade Federal de Santa Maria}
\def\ufsmIngles{Universidade Federal de Santa Maria} 

\newcommand{\course}[1]{\gdef\@course{#1}}
\newcommand{\altcourse}[1]{\gdef\@altcourse{#1}}
\newcommand{\institute}[1]{\gdef\@institute{#1}}
\newcommand{\degree}[1]{\gdef\@degree{#1}}

\newcommand{\trabalhoNumero}[1] {\gdef\@trabalhoNumero{#1}}

\newcommand{\location}[3]{
        \gdef\@city{#1}
        \gdef\@state{#2}
        \gdef\@country{#3}
}
\location{Santa~Maria}{RS}{Brasil}

%%=============================================================================
%% Heran�a da classe 'report' e inclus�o de pacotes e arquivos necess�rios
%%=============================================================================
\LoadClass[12pt,a4paper,oneside]{report}
\RequirePackage[english,brazilian]{babel}
\input{brazilian.babel} % defini��es para textos em portugu�s
\input{english.babel}   % defini��es para textos em ingl�s
\if@english
        \selectlanguage{brazilian}
        \AtBeginDocument{\selectlanguage{brazilian}}
\fi
\RequirePackage{lastpage}
\RequirePackage{indentfirst} 
\RequirePackage{natbib}


%%=============================================================================
%% Margens do texto
%%=============================================================================
\newlength{\mdt@topmargin}\setlength{\mdt@topmargin}{30mm}
\newlength{\mdt@pagenummargin}\setlength{\mdt@pagenummargin}{20mm}
\newlength{\mdt@bottommargin}\setlength{\mdt@bottommargin}{20mm}
\newlength{\mdt@sidemargin}\setlength{\mdt@sidemargin}{30mm}
\setlength{\marginparwidth}{25mm}

% ajuste das medidas verticais
\setlength{\topmargin}{\mdt@pagenummargin}
        \addtolength{\topmargin}{-1em} % isto � +- a altura do n�m. da p�gina
        \addtolength{\topmargin}{-1in}
\setlength{\headheight}{1em}
\setlength{\headsep}{\mdt@topmargin}
        \addtolength{\headsep}{-\mdt@pagenummargin}
\setlength{\textheight}{\paperheight}
        \addtolength{\textheight}{-\mdt@topmargin}
        \addtolength{\textheight}{-\mdt@bottommargin}
\setlength{\footskip}{\mdt@bottommargin}

% ajuste das medidas horizontais
\setlength{\oddsidemargin}{\mdt@sidemargin}
        \addtolength{\oddsidemargin}{-1in}
\setlength{\evensidemargin}{\oddsidemargin}
\setlength{\textwidth}{\paperwidth}
        \addtolength{\textwidth}{-2\mdt@sidemargin}
\setlength{\marginparsep}{\mdt@sidemargin}
        \addtolength{\marginparsep}{-\marginparwidth}
        \setlength{\marginparsep}{.5\marginparsep}

%%=============================================================================
%% Numera��o das p�ginas
%%=============================================================================
\def\ps@mdtufsm{
        \let\@oddfoot\@empty
        \let\@evenfoot\@empty
        \def\@evenhead{{\small\thepage}}
        \def\@oddhead{\hspace*{\fill}{\small\thepage}}
}
\pagestyle{empty}
\let\ps@plain\ps@empty

%%=============================================================================
%% Defini��o de autor(es), orientador, co-orientador e banca examinadora
%%=============================================================================
\newcounter{ii@authorcount}
\renewcommand{\author}[2]{
        \addtocounter{ii@authorcount}{1}
        \expandafter\gdef\csname @author\alph{ii@authorcount}last\endcsname{#1}
        \expandafter\gdef\csname @author\alph{ii@authorcount}first\endcsname{#2}
        \expandafter\gdef\csname @author\alph{ii@authorcount}\endcsname{#2 #1}
}
\DeclareRobustCommand{\advisor}[3][]{
        \gdef\@advisorlast{#2}
        \gdef\@advisorfirst{#3}
        \gdef\@advisor{#1~#3 #2}
}
\DeclareRobustCommand{\coadvisor}[3][]{
        \gdef\@coadvisorlast{#2}
        \gdef\@coadvisorfirst{#3}
        \gdef\@coadvisor{#1~#3 #2}
}
\newcounter{ii@committeecount}
\DeclareRobustCommand{\committee}[3][]{
  \addtocounter{ii@committeecount}{1}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}last\endcsname{#2}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}first\endcsname{#3}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}\endcsname{#1~#3 #2}
}



%%=============================================================================
%% Defini��o das palavras-chave (m�x. 10)
%%=============================================================================
\newcounter{kwcounter}
\newcommand{\keyword}[1]{
        \addtocounter{kwcounter}{1}
        \expandafter\gdef\csname keyw\alph{kwcounter}\endcsname{#1}
}
\newcommand{\@cipkw}{%
        \@ifundefined{keywa}{}{1.~\expandafter\MakeUppercase\keywa\@. }%
        \@ifundefined{keywb}{}{2.~\expandafter\MakeUppercase\keywb\@. }%
        \@ifundefined{keywc}{}{3.~\expandafter\MakeUppercase\keywc\@. }%
        \@ifundefined{keywd}{}{4.~\expandafter\MakeUppercase\keywd\@. }%
        \@ifundefined{keywe}{}{5.~\expandafter\MakeUppercase\keywe\@. }%
        \@ifundefined{keywf}{}{6.~\expandafter\MakeUppercase\keywf\@. }%
        \@ifundefined{keywg}{}{7.~\expandafter\MakeUppercase\keywg\@. }%
        \@ifundefined{keywh}{}{8.~\expandafter\MakeUppercase\keywh\@. }%
        \@ifundefined{keywi}{}{9.~\expandafter\MakeUppercase\keywi\@. }%
        \@ifundefined{keywj}{}{10.~\expandafter\MakeUppercase\keywj\@. }%
}
\newcommand{\@abstractkw}{%
        \@ifundefined{keywa}{}{\expandafter\MakeUppercase\keywa}%
        \@ifundefined{keywb}{}{, \keywb}%
        \@ifundefined{keywc}{}{, \keywc}%
        \@ifundefined{keywd}{}{, \keywd}%
        \@ifundefined{keywe}{}{, \keywe}%
        \@ifundefined{keywf}{}{, \keywf}%
        \@ifundefined{keywg}{}{, \keywg}%
        \@ifundefined{keywh}{}{, \keywh}%
        \@ifundefined{keywi}{}{, \keywi}%
        \@ifundefined{keywj}{}{, \keywj}%
}

%%=============================================================================
%% Redefini��o da data (para ter dia, m�s e ano separados)
%%=============================================================================
\renewcommand{\date}[3]{
  \gdef\@publday{#1}
  \gdef\@publmonth{#2}
  \gdef\@publyear{#3}
}
\date{\monthname}{\number\year}

%%=============================================================================
%% Defini��es comuns � capa e � folha de rosto.
%% Os dados espec�ficos do tipo de trabalho (disserta��o, tese, etc.) s�o
%% informados definindo-se o comando \titlepagespecificinfo. Veja os
%% arquivos .def inclu�dos com a classe.
%%=============================================================================
\newlength{\titlepagewinwidth}\setlength{\titlepagewinwidth}{135mm}
\newlength{\titlepagewinheight}\setlength{\titlepagewinheight}{70mm}
\newlength{\titlepagewinvoffset}\setlength{\titlepagewinvoffset}{69mm}
%\newlength{\titlepagewinvoffset}\setlength{\titlepagewinvoffset}{112mm}


\renewcommand{\maketitle}{ % capa e folha de rosto
        \begin{titlepage}
                \thetitlepage
        \end{titlepage}
        \clearpage
        \secondtitlepage
        \setcounter{page}{2}
}


%%=============================================================================
%% Capa
%%=============================================================================
\newcommand{\thetitlepage}{ 
  \noindent
  % cabe�alho
  \parbox[t]{\textwidth}{
    \centering
    \vbox to \titlepagewinvoffset{
      {\large\textbf{\MakeUppercase{\ufsm}}}\\
      {\large\textbf{\MakeUppercase{\@institute}}}\\
      {\large\textbf{\MakeUppercase{\@course}}}\\
    }
    \vfill
    % t�tulo
    \parbox[b]{\titlepagewinwidth}{%
      \centering
      \vbox to \titlepagewinheight{%
        \vspace*{\fill}
        {\LARGE\textbf{\MakeUppercase{\@title}}\par}
        \vfill
        \centerline{\large\textbf{\MakeUppercase{\doctypename}}}
      }
    }
  }
  \vfill
  \centerline{\Large\textbf{\@authora}}
  \vfill
  \centerline{\large\textbf{\@city, \@state, \@country}}
  \vspace{12pt}
  \centerline{\large\textbf{\@publyear}}
}



%%=============================================================================
%% Folha de rosto
%%=============================================================================
\newcommand{\secondtitlepage}{ 
  \noindent
  \parbox[t]{\textwidth}{
    \centering
    {\Large\textbf{\MakeUppercase{\@title}}\par}
  }
  \vfill\vfill\vfill
  \centerline{\large\textbf{por}}
  \vfill\vfill
  \noindent
  \parbox[t]{\textwidth}{
    \centering
    {\large\textbf{\@authora}}
  }
  \vfill\vfill
  \noindent
  \parbox[t]{\textwidth}{
    \centering
    {\large\titlepagespecificinfo}
  }
  \vfill\vfill\vfill
  \noindent
  \parbox[t]{\textwidth}{
    \centering
    \@ifundefined{@advisor}{}{
      \vfill{\large\textbf{\advisorname:~\@advisor}}}
    \@ifundefined{@coadvisor}{}{
      \\[6ex]{\large\textbf{\coadvisorname:~\@coadvisor}}}
  }
  \vfill\vfill\vfill
  \centerline{\large\textbf{\@trabalhoNumero}}
  \centerline{\large\textbf{\@city, \@state, \@country}}
  \vspace{12pt}
  \centerline{\large\textbf{\@publyear}}
}











%%=============================================================================
%% Defini��o padr�o da folha de aprovacao.
%%=============================================================================
\newcommand{\makeapprove}{%
  \clearpage
  \theapprovepage
}
\newcommand{\theapprovepage}{%
  \noindent
  \parbox[t]{\textwidth}{%
    \centering
    {\large\textbf{\ufsm}} \\
    {\large\textbf{\@institute}} \\
    {\large\textbf{\@course}} \\
  }
  \vfill
  \noindent\@authorbox
  \vfill
  \noindent\approvecommitteeinfo
  \vfill
  \vfill
  \centerline{\@city,~\@date.}
}

\def\@authorbox{%
  \parbox[t]{\textwidth}{%
    \centering
    \vspace{2cm}
    \large{A Comiss\~{a}o Examinadora, abaixo assinada,} \\
	  {aprova \doctypearticle~\doctypename} \\
	  \vspace{1,5cm}
	  \MakeUppercase{\textbf{\@title}}\\
	  \vspace{1cm}
	  elaborad\doctypearticle~ por\\
	  \textbf{\@authora}\\
	  \@ifundefined{@authorb}{}{\textbf{\@authorb}\\}%
	  \@ifundefined{@authorc}{}{\textbf{\@authorc}\\}%
	  \@ifundefined{@authord}{}{\textbf{\@authord}\\}%
	  \vspace{1cm}
		 {como requisito parcial para obten��o do grau de} \\
		 \textbf{\@degree}  
  }
}



\newcommand{\approvecommitteeinfo}{%
  \parbox[t]{\textwidth}{
    \large
    \centerline{\textbf{COMISS�O EXAMINADORA:}}
    \vfill
    \vspace{1,2cm}
    \@ifundefined{@advisor}{}{\centerline{\textbf{\@advisor}}}
    \centerline{(Presidente/Co-orientador)}
    \vspace{1,2cm}
    \@ifundefined{@committeea}{}{\centerline{\textbf{\@committeea}}}
    \vspace{1,2cm}
    \@ifundefined{@committeeb}{}{\centerline{\textbf{\@committeeb}}}
  }
}




%%=============================================================================
%% Ajuste do sum�rio (sem endenta��o) 
%%=============================================================================
\newlength{\ii@chapnumindent}\setlength{\ii@chapnumindent}{1.8em}
\renewcommand\@dotsep{1.5}
\renewcommand*{\l@chapter}[2]{
  \addpenalty{-\@highpenalty}
  \vskip -1ex \@plus\p@
  \setlength\@tempdima{1.5em}
  \begingroup
  \parindent \z@ \rightskip \@pnumwidth
  \parfillskip -\@pnumwidth
  \leavevmode\bfseries
  \advance\leftskip\@tempdima
  \hskip -\leftskip
  %      esta � a definicao original da classe report:
  %      #1\nobreak\hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
  \@dottedtocline{0}{0em}{\ii@chapnumindent}{\uppercase{#1}}{#2}
  %        \@dottedtocline{0}{0em}{\ii@chapnumindent}{\MakeUppercase{#1}}{#2}
  \penalty\@highpenalty
  \endgroup
}
\renewcommand*{\l@section}[2]
              {\@dottedtocline{1}{0em}{2.8em}{\textbf{#1}}{\hss #2}}
\renewcommand*{\l@subsection}{\@dottedtocline{2}{0em}{3.7em}}
\newif\if@aftertoc
\@aftertocfalse
\renewcommand{\tableofcontents}{%
        \chapter*{\contentsname}
        \@starttoc{toc}\@aftertoctrue%
}

%%=============================================================================
%% Defini��o das listas de abreviaturas e siglas e de s�mbolos
%%=============================================================================
\newenvironment{listofabbrv}[1]{
        \chapter*{\listabbrvname}
        \begin{list}{\textbf{??}}{
                \settowidth{\labelwidth}{#1}
                \setlength{\labelsep}{1em}
                \setlength{\itemindent}{0mm}
                \setlength{\leftmargin}{\labelwidth}
                \addtolength{\leftmargin}{\labelsep}
                \setlength{\rightmargin}{0mm}
                \setlength{\itemsep}{.1\baselineskip}
                \renewcommand{\makelabel}[1]{\makebox[\labelwidth][l]{##1}}
        }
}{
        \end{list}
}
\newenvironment{listofsymbols}[1]{
        \chapter*{\listsymbolname}
        \begin{list}{\textbf{??}}{
                \settowidth{\labelwidth}{#1}
                \setlength{\labelsep}{1em}
                \setlength{\itemindent}{0mm}
                \setlength{\leftmargin}{\labelwidth}
                \addtolength{\leftmargin}{\labelsep}
                \setlength{\rightmargin}{0mm}
                \setlength{\itemsep}{.1\baselineskip}
                \renewcommand{\makelabel}[1]{\makebox[\labelwidth][l]{##1}}
        }
}{
        \end{list}
}

%%=============================================================================
%% Resumo (abstract) e Abstract (englishabstract)
%%=============================================================================
\renewenvironment{abstract}{%
  \chapter*{\abstractname}
  \noindent\abstractheaderbox
  \indent
}{%
  \par\vfill
  \noindent\textbf{\keywordsname:} \@abstractkw.
}

\newcommand{\abstractheaderbox}{%
  \parbox[t]{\textwidth}{
    \centering
    \doctypename\\
    \@course\\
    \ufsm\\~\\
    \textbf{\MakeUppercase{\@title}}\\
    \authorname:~\@authora\\
    \@ifundefined{@advisor}{}{\advisorname:~\@advisor\\}
    \@ifundefined{@coadvisor}{}{\coadvisorname:~\@coadvisor\\}
    Local e data da defesa:~\@city,~\@date.\\~\\
  }
}

\newenvironment{englishabstract}[3]{
        \if@openright\cleardoublepage\else\clearpage\fi%
        \gdef\@englishkeywords{#3}
        \begin{otherlanguage}{english}
          \chapter*{\abstractname}
          \noindent\englishabstractheaderbox{#1}{#2}
          \indent
}{
          \par\vfill
          \noindent\textbf{\keywordsname:} \@englishkeywords.
       \end{otherlanguage}
}

\newcommand{\englishabstractheaderbox}[2]{%
  \parbox[t]{\textwidth}{
    \centering
    \doctypenameIngles\\
    {#2}\\
    \ufsmIngles\\~\\
    \textbf{\MakeUppercase{#1}}\\
    \authorname:~\@authora\\
    \@ifundefined{@advisor}{}{\advisorname:~\@advisor\\}
    \@ifundefined{@coadvisor}{}{\coadvisorname:~\@coadvisor\\}
    ~\\
  }
}

%%=============================================================================
%% Formata��o dos cap�tulos.
%% Os comandos \@chapter e \@schapter s�o redefinidos para que a
%% numera��o de p�ginas seja correta (deve aparecer somente a partir do
%% primeiro cap�tulo do texto, mas n�o nas p�ginas iniciais de
%% cap�tulos).
%% Os comandos \@makechapterhead e \@makeschapterhead fazem a
%% formata��o dos t�tulos propriamente ditos.
%%=============================================================================
\newcommand{\ii@chapterfont}{\@setfontsize\Large{16}{20}}
\def\@chapapp{}
\def\@chapter[#1]#2{%
        \refstepcounter{chapter}%
        \addcontentsline{toc}{chapter}{\protect\numberline{\@chapapp\thechapter}#1}%
        \addtocontents{lof}{\protect\addvspace{10\p@}}%
        \addtocontents{lot}{\protect\addvspace{10\p@}}%
        \pagestyle{mdtufsm}\let\ps@plain\ps@mdtufsm%
        \@makechapterhead{#2}\@afterheading
}
\def\@makechapterhead#1{%
        \vspace*{90pt}
        \reset@font\noindent{\ii@chapterfont\centering\textbf{\@chapapp\thechapter\hspace{1em}\MakeUppercase{#1}}\par}
        \addvspace{42pt}
}
\def\@schapter#1{%
        \if@aftertoc\addcontentsline{toc}{chapter}{#1}\fi%
        \addtocontents{lof}{\protect\addvspace{10\p@}}%
        \addtocontents{lot}{\protect\addvspace{10\p@}}%
        \@makeschapterhead{#1}\@afterheading
}
\def\@makeschapterhead#1{%
        \vspace*{90pt}
        \reset@font\centerline{\ii@chapterfont\centering\textbf{\MakeUppercase{#1}}}
        \addvspace{42pt}
}
\renewcommand{\appendix}[1][\appendixname]{%
        \setcounter{chapter}{0}%
        \renewcommand{\thechapter}{\Alph{chapter}}%
        \setcounter{tocdepth}{0}%
        \@captionlistingfalse%
        \gdef\@chapapp{\MakeUppercase{#1}\space}%
        \addtocontents{toc}{%
          \settowidth{\ii@chapnumindent}{{\textbf{\@chapapp}}}
          \addtolength{\ii@chapnumindent}{1.8em}
        }%
}
\newcommand{\annex}{\appendix[\annexname]}

%%=============================================================================
%% Formata��o das se��es
%%=============================================================================
\setcounter{secnumdepth}{4}
\renewcommand{\section}{%
        \@startsection{section}{1}{0pt}{18pt}{9pt}{\reset@font\large\bfseries}
}
\renewcommand{\subsection}{%
        \@startsection{subsection}{2}{0pt}{12pt}{6pt}{\reset@font\bfseries}
}
\renewcommand{\subsubsection}{%
        \@startsection{subsubsection}{3}{0pt}{12pt}{6pt}{\reset@font\itshape}
}
\renewcommand{\paragraph}{%
        \@startsection{paragraph}{4}{0pt}{12pt}{6pt}{\reset@font}
}
\renewcommand{\subparagraph}{%
        \@startsection{subparagraph}{5}{0pt}{1ex}{-1em}{\reset@font\bfseries}
}

%%=============================================================================
%% Redefini��o do conte�do das listas de figuras e tabelas
%%=============================================================================
\newif\if@captionlisting\@captionlistingtrue
\long\def\@caption#1[#2]#3{%
  \par
  \if@captionlisting%
    \addcontentsline{\csname ext@#1\endcsname}{#1}%
        {\protect\numberline{\csname fnum@#1\endcsname~--}{\ignorespaces #2}}%
  \fi%
  \begingroup
    \@parboxrestore
    \if@minipage
      \@setminipage
    \fi
    \normalsize
    \@makecaption{\csname fnum@#1\endcsname}{\ignorespaces #3}\par
  \endgroup%
}
\renewcommand*{\l@figure}{\@dottedtocline{1}{0em}{5.5em}}
\let\l@table\l@figure

%%=============================================================================
%% Redefini��o do formato de cita��es longas
%%=============================================================================
\renewenvironment{quote}{
        \small
        \begin{list}{}{\setlength{\leftmargin}{40mm}\item\relax}
}{
        \end{list}
}

%%=============================================================================
%% Ajuste do formato das cita��es e refer�ncias
%%=============================================================================
\setlength{\bibhang}{0pt}
\let\cite\citep

