import 'package:flutter/material.dart';
import 'quartoTab.dart';
import 'terzoTab.dart';

Color colorePizza = Colors.red;

class PageBar extends StatefulWidget {
  final Function callback;

  PageBar(this.callback);

  @override
  _PageBarState createState() => _PageBarState();
}

class _PageBarState extends State<PageBar> {
  @override
  Widget build(BuildContext context) {
    return Row(
      children: <Widget>[
        //_primo tab
        Expanded(
            child: FlatButton(
          onPressed: () {
            //_questo callback serve per cambiare il colore (contiene un setState nella pagina HomePage.dart)
            this.widget.callback(Colors.blue);
          },
          onLongPress: () => null,
          child: Text("prima pagina"),
          splashColor: Colors.lightBlue[50],
        )),

        //_secondo tab
        Expanded(
            child: FlatButton(
          onPressed: () {
            //_questo callback serve per cambiare il colore (contiene un setState nella pagina HomePage.dart)
            this.widget.callback(Colors.red);
          },
          onLongPress: () => null,
          child: Text("seconda pagina"),
          splashColor: Colors.lightBlue[50],
        )),

        //_terzo tab
        Expanded(
          child: FlatButton(
          onPressed: () {
            Navigator.push(context, MaterialPageRoute(builder: (context) => TerzoTab()));
          },
          onLongPress: null,
          child: Text("terza pagina"),
          splashColor: Colors.lightBlue[50],
        )),

      ],
    );
  }
}
