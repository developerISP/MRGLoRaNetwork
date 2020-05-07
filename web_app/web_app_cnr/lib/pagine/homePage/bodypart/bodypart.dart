import 'dart:math';

import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart';
import 'datiCorrenti/datoTemperaturaCorrente.dart';
import 'datiCorrenti/datoOzonoCorrente.dart';

import 'Graph.dart';

class Data{
  String date;
  double value;

  Data(this.date, this.value);
}



class BodyPart extends StatefulWidget {
  BodyPart({Key key}) : super(key: key);

  @override
  _BodyPartState createState() => _BodyPartState();
}

class _BodyPartState extends State<BodyPart>{
  List<Data> data = [
    for(int i=0; i<10; i++)
    //for(int j=0; j<24; j++)
      new Data("$i/10/2020", Random().nextInt(100).toDouble() ) 
  ];
  
  int _indice;
  double _valoreFinale;

  Data _datoSelOzon;
  Data _datoSelTemp;
  Data _datoSelUmid;
  Data _datoSelwind;

  @override
  void initState() { 
    super.initState();
    this._indice = this.data.length-1;
    this._valoreFinale = this.data[this._indice].value.toDouble();

    this._datoSelOzon = this.data[this._indice];
    this._datoSelTemp = this.data[this._indice];
    this._datoSelUmid = this.data[this._indice];
    this._datoSelwind = this.data[this._indice];
  }

/*
  void goBack(){
    setState(() {
      if(this._indice > 0){
        this._indice--;
        _valoreFinale = data[this._indice].value.toDouble();
      }
    });
  }

  void goForward(){
    setState(() {
      if(this._indice < data.length-1){
        this._indice++;
        _valoreFinale = data[this._indice].value.toDouble();
      }

    });
  }
*/

  @override
  Widget build(BuildContext context) {

    double width = 250;
    double height = 250;


    return Column(
      children: <Widget>[

        Container(//# Ozono
          margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

          child: Column(
            children: <Widget>[

              Container(//! titolo OZONO
                margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
                child: Center(child: Text("Ozon data", style: TextStyle(fontSize: 20)))
              ),

              Container(//! dati OZONO
                margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

                height: 400,

                child: Row(
                  children: <Widget>[

                    Expanded(//_grafico
                      flex: 6,
                      child: Center(child: DateTimeComboLinePointChart.withDate(data, this._datoSelOzon, charts.MaterialPalette.blue.shadeDefault))
                    ),

                    Expanded(
                      flex: 4,
                      child: DatoOzonoCorrente(
                        "mg/l",
                        data,
                        Colors.lightBlue,
                        width,
                        height,
                        datoSelezionato: (int indice){
                          setState(() {
                            this._datoSelOzon = this.data[indice];
                          });
                        },

                      ),
                    )


                  ],
                ),
              ),

            ],
          ),

        ),

        Container(//# Temperatura
          margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

          child: Column(
            children: <Widget>[

              Container(//! titolo TEMPERATURA
                margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
                child: Center(child: Text("Temperature data", style: TextStyle(fontSize: 20)))
              ),

              Container(//! dati TEMPERATURA
                margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

                height: 400,

                child: Row(
                  children: <Widget>[

                    Expanded(//_dati attuali
                      flex: 4,
                      child: DatoTemperaturaCorrente(
                        "°C",
                        data,
                        Colors.red,
                        width,
                        height,
                        datoSelezionato: (int indice){
                          setState(() {
                            this._datoSelTemp = this.data[indice];
                          });
                        },

                      )
                      
                    ),

                    Expanded(//_grafico
                      flex: 6,
                      child: Center(child: DateTimeComboLinePointChart.withDate(data, this._datoSelTemp, charts.MaterialPalette.red.shadeDefault))
                    ),



                  ],
                ),
              ),

            ],
          ),
        ),

        Container(//# Umidità
          margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

          child: Column(
            children: <Widget>[

              Container(//! titolo UMIDITA'
                margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
                child: Center(child: Text("Umidity data", style: TextStyle(fontSize: 20)))
              ),

              Container(//! dati UMIDITA'
                margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

                height: 400,

                child: Row(
                  children: <Widget>[

                    Expanded(//_grafico
                      flex: 6,
                      child: Center(child: DateTimeComboLinePointChart.withDate(data, this._datoSelUmid, charts.MaterialPalette.yellow.shadeDefault))
                    ),

                    Expanded(//_dati attuali
                      flex: 4,
                      child: DatoOzonoCorrente(
                        "mg/l",
                        data,
                        Colors.amber,
                        width,
                        height,
                        datoSelezionato: (int indice){
                          setState(() {
                            this._datoSelUmid = this.data[indice];
                          });
                        },
                        
                      )
                      
                    ),


                  ],
                ),
              ),


            ],
          ),

        ),

        Container(//# Vento
          margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

          child: Column(
            children: <Widget>[

              Container(//! titolo VENTO
                margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
                child: Center(child: Text("Wind data", style: TextStyle(fontSize: 20)))
              ),

              Container(//! dati VENTO
                margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

                height: 400,

                child: Row(
                  children: <Widget>[

                    Expanded(//_dati attuali
                      flex: 4,
                      child: DatoOzonoCorrente(
                        "mg/l",
                        data,
                        Colors.green,
                        width,
                        height,
                        datoSelezionato: (int indice){
                          setState(() {
                            this._datoSelwind = this.data[indice];
                          });
                        },

                      )
                       
                    ),

                    Expanded(//_grafico
                      flex: 6,
                      child: Center(child: DateTimeComboLinePointChart.withDate(data, this._datoSelwind, charts.MaterialPalette.green.shadeDefault))
                    ),

                  ],
                ),
              ),


            ],
          )

        ),


      ],
    );
  }
}
