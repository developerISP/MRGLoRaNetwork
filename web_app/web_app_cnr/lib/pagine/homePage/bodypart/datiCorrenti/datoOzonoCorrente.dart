import 'package:flutter/material.dart';
import '../bodypart.dart';
import '../indicatori/IndicatoreOzono.dart';


class DatoOzonoCorrente extends StatefulWidget {
  final String _unit;
  final List<Data> _data;
  final Color _colore;
  final double _width;
  final double _height;

  final Function datoSelezionato;

  DatoOzonoCorrente(this._unit, this._data, this._colore, this._width, this._height, {Key key, this.datoSelezionato}) : super(key: key);

  @override
  _DatoOzonoCorrenteState createState() => _DatoOzonoCorrenteState();
}


class _DatoOzonoCorrenteState extends State<DatoOzonoCorrente> {
  int indice;
  double valoreFinale;

  @override
  void initState() {
    print(widget._data.length-1);
    this.indice = widget._data.length-1;
    this.valoreFinale = widget._data[this.indice].value;
    super.initState();
  }

  void goBack(){
    setState(() {
      if(indice > 0){
        indice--;
        valoreFinale = widget._data[this.indice].value.toDouble();
        widget.datoSelezionato(this.indice);
      }
    });
  }

  void goForward(){
    setState(() {
      if(indice < widget._data.length-1){
        indice++;
        valoreFinale = widget._data[indice].value.toDouble();
        widget.datoSelezionato(this.indice);
      }

    });
  }
  
  @override
  Widget build(BuildContext context) {
    return Container(
        child: Column(
          children: <Widget>[
            Expanded(
              flex: 8,
              child:  TweenAnimationBuilder(
                duration: const Duration(seconds: 1),
                curve: Curves.easeOutQuad,
                tween: Tween<double>(begin: 0, end: this.valoreFinale),
                builder: (_, _value, __) {

                  return Container(//_indicatore analogico
                    child: CustomPaint(
                      painter: IndicatoreOzono(_value, 100, widget._colore),
                      size: Size(widget._width, widget._height),

                      child: Container(
                        width: widget._width,
                        height: widget._height,

                        child: Center(//_l'interno dell'indicatore
                          child: Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: <Widget>[
                              Text(
                                (this.indice == widget._data.length-1 ? "today's" : widget._data[indice].date) + " value",
                                style: TextStyle(fontSize: 18),
                              ),

                              Row(
                                mainAxisAlignment: MainAxisAlignment.center,
                                children: <Widget>[
                                  Text(//_valore
                                    "${_value.toInt()}",
                                    style: TextStyle(fontSize: 60),
                                  ),

                                  Text(//_unita di misura
                                    widget._unit,
                                    style: TextStyle(fontSize: 25, fontWeight: FontWeight.w500),
                                  )
                                ],
                              ),

                            ],
                          ),
                        ),
                      ),
                    ),

                    alignment: Alignment.center,
                  );


                },
              ),

            ),

            Expanded(
              flex: 2,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[

                  FlatButton(
                    onPressed: () { goBack(); },
                    onLongPress: () => null,
                    child: Icon(Icons.arrow_back_ios),

                    splashColor: Colors.lightBlue,
                    highlightColor: Colors.lightBlue,
                    hoverColor: Colors.lightBlue[100],

                  ),
                  FlatButton(
                    onPressed: () { goForward(); },
                    onLongPress: () => null,
                    child: Icon(Icons.arrow_forward_ios),

                    splashColor: Colors.lightBlue,
                    highlightColor: Colors.lightBlue,
                    hoverColor: Colors.lightBlue[100],

                  )

                ],
              )
            )


          ],
        ),
      );
  }
}