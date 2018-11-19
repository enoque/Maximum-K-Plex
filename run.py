import os
import signal
import sys

instancias = [
"brock200_2",
"brock200_1",
"brock200_3",
"brock200_4",
"brock400_1",
"brock400_2",
"brock400_3",
"brock400_4",
"brock800_1",
"brock800_2",
"brock800_3",
"brock800_4",

"C125.9",
"C250.9",
"C500.9",
"C1000.9",
"C2000.5",
"C2000.9",
"C4000.5",

"c-fat200-1",
"c-fat200-2",
"c-fat200-5",
"c-fat500-1",
"c-fat500-2",
"c-fat500-5",
"c-fat500-10",

"DSJC500_5",
"DSJC1000_5",

"gen200_p0.9_44",
"gen200_p0.9_55",
"gen400_p0.9_55",
"gen400_p0.9_65",
"gen400_p0.9_75",

"hamming6-2",
"hamming6-4",
"hamming8-2",
"hamming8-4",
"hamming10-2",
"hamming10-4",

"johnson8-2-4",
"johnson8-4-4",
"johnson16-2-4",
"johnson32-2-4",

"keller4",
"keller5",
"keller6",

"MANN_a9",
"MANN_a27",
"MANN_a45",
"MANN_a81",

"p_hat300-1",
"p_hat300-2",
"p_hat300-3",
"p_hat500-1",
"p_hat500-2", #parou aqui
"p_hat500-3",
"p_hat700-1",
"p_hat700-2",
"p_hat700-3",
"p_hat1000-1",
"p_hat1000-2",
"p_hat1000-3",
"p_hat1500-1",
"p_hat1500-2",
"p_hat1500-3",

"san200_0.7_1",
"san200_0.7_2",
"san200_0.9_1",
"san200_0.9_2",
"san200_0.9_3",
"san400_0.5_1",
"san400_0.7_1",
"san400_0.7_2",
"san400_0.7_3",
"san400_0.9_1",
"san1000",

"sanr200_0.7",
"sanr200_0.9",
"sanr400_0.5",
"sanr400_0.7",
]


arquivo = ""

def signal_handler(sig, frame):
        print('You pressed Ctrl+C!')
	os.system("rm resultados/%s_enoque.out" % arquivo)
        sys.exit(0)



for inst in instancias:
	arquivo = inst
	if not os.path.isfile("resultados/%s_enoque.out" % inst):
		print ("./max-kplex 3600 2 entradas/%s.clq > resultados/%s_enoque.out" % (inst,inst))			
		os.system("./max-kplex 3600 2 entradas/%s.clq > resultados/%s_enoque.out " % (inst,inst) )
		signal.signal(signal.SIGINT, signal_handler)
		signal.pause()
	#if not os.path.isfile("resultados/%s_mauro.clq" % inst):
		#print ("./RDPLEX 3600 2 entradas/%s.clq > resultados/%s_mauro.out" % (inst,inst))			
		#os.system("./max-kplex 3600 2 entradas/%s.clq > resultados/%s_mauro.out " % (inst,inst) )


	


