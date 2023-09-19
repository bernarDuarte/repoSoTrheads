using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Security.Cryptography.X509Certificates;

namespace BubbleSort
{
    class BubbleSort
    {
        static void Main( string[] args )
        {

            List<int> BubbleSortPadrao( List<int> list )
            {                                             
                int temp;

                //Loop do sort
                for ( int j = 0; j <= list.Count - 2; j++ )
                {
                    for ( int i = 0; i <= list.Count - 2; i++ )
                    {
                        if ( list[i] > list[i + 1] )
                        {
                            temp = list[i + 1];
                            list[i + 1] = list[i];
                            list[i] = temp;
                        }
                    }
                }               

                return list;                
            }

            List<int> BubbleSortMultiThread( List<int> list )
            {
                //Cria uma lista com os menores valores
                List<int> listMenores = new List<int>();
                foreach ( int i in list )
                {
                    if( i <= list.Max() / 2 )
                        listMenores.Add( i );
                }

                //Remove os menores valores da lista principal
                list = list.Except(listMenores).ToList();


                //Uma Thread para cada lista
                Thread t1 = new Thread( () => BubbleSortPadrao( list ) );
                Thread t2 = new Thread( () => BubbleSortPadrao( listMenores ) );

                //Inicia a execução das Threads
                t1.Start();
                t2.Start();

                //Espera que a execução seja finalizada
                t1.Join();
                t2.Join();

                //Une as 2 listas
                return listMenores.Union(list).ToList();
            }


            while ( true )
            {
                int TamanhoLista = 0;
                Console.WriteLine( " Tamanho da lista de inteiros: " );
                TamanhoLista = Convert.ToInt32( Console.ReadLine() );

                //Cria uma lista com números de TamanhoLista até 1
                List<int> lista = new List<int>();
                for ( int j = TamanhoLista; j >= 1; j-- )
                {
                    lista.Add( j );
                }

                Console.WriteLine(" 1 - Sort Padrão");
                Console.WriteLine(" 2 - Sort MultiThread");
                

                if ( Console.ReadLine() == "1" )
                {
                    Console.Clear();
                    //Cria contardor de tempo
                    Stopwatch tempo = new Stopwatch();
                    tempo.Start();

                    List<int> resultado = BubbleSortPadrao( lista );                    

                    //Para o contador e exibe o tempo
                    tempo.Stop();
                    Console.WriteLine( string.Join( " ,", resultado ) );
                    Console.WriteLine( tempo.Elapsed );
                }
                else
                {
                    Console.Clear();
                    //Cria contardor de tempo
                    Stopwatch tempo = new Stopwatch();
                    tempo.Start();

                    List<int> resultado = BubbleSortMultiThread( lista );                    

                    //Para o contador o exibe o tempo
                    tempo.Stop();
                    Console.WriteLine( string.Join( " ,", resultado ) );
                    Console.WriteLine( tempo.Elapsed );
                }                    
            }


        }
    }
}
