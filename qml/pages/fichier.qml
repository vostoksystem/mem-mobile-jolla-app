import QtQuick 2.2
import Sailfish.Silica 1.0
import "../composents"
import "../hints"

Page {
    id:page
    width: root.width

    property bool active: status===PageStatus.Active
    property string titre
    property string source
    property var _etat

    property real _timestamp :0

    //-----------------------------------------
    function rafraichir(mode) {
        if(mode === false) {
            if(_timestamp+ 120000 > Date.now()) {
                c.requestPaint();
                return; // pas la peine, fait il y a moins de 2min
            }
        }

        var tmp=FichierService.utilisation(source);
        if(_etat && mode === false) {
            // verif si à jour

            if(_etat.actuel + 10240>tmp.actuel )  {
                c.requestPaint();
                return;
            }
        }
        _etat=tmp;
        _timestamp= Date.now();

        listModel.remplir(FichierService.statistique(source,mode));
        occupation.taille = _etat.pourcentage *100;
    }

    ListModel {
        id: listModel

        function remplir(l) {
            clear();
            for(var i in l) {
                append({
                           nom:l[i].nom,
                           valeur:l[i].valeur,
                           couleur:l[i].couleur,
                           identifiant:l[i].identifiant,
                           info:l[i].info,
                           pourcentage:_etat.total>0 ? l[i].valeur/_etat.total :0
                       });
            }
     //       console.log("fin remplissage")
        }
    }

    SilicaFlickable {
        width: parent.width
        anchors.fill: parent
        visible: listModel.count>0

        PullDownMenu {
            MenuItem {
                text: vi18n.get("menu.config")
                onClicked: pageStack.push(Qt.resolvedUrl("config.qml"))
            }
            MenuItem {
                text: vi18n.get("menu.recharger")
                onClicked:{
                    rafraichir(true);
                }
            }
        }

        VerticalScrollDecorator {  }

        PageHeader {
            id:ph
            title: vi18n.get(titre)
        }

        Camembert {
            id:c
            visible: listModel.count>0
            model:  listModel
            anchors.top: ph.bottom

            Label {
                id:occupation
                property real taille: 0

                anchors.centerIn: parent
                text: taille.toFixed(2) + "%"
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge

                Behavior on taille {
                    NumberAnimation { duration: 1500; easing.type: Easing.Linear }
                }
            }
        }

        ColumnView {
            width: parent.width
            x:Theme.horizontalPageMargin
            itemHeight: Theme.itemSizeSmall
            anchors {
                top: c.bottom
                topMargin: Theme.paddingLarge
            }
            model: listModel
            delegate: RowSerieDelegate{}
        }
    }

    Attente {
        enabled: listModel.count===0
    }

    Hint {
        id:hin1
        clee : "/vostok/memory/hint/liste"
        texte: vi18n.get("hint.liste")
        direction:TouchInteraction.Down
        interactionMode:TouchInteraction.Swipe
    }

    onActiveChanged: {
        if(active) {
            rafraichir(false);
        }
    }

    Component.onCompleted:  {
        hin1.lancer();
        rafraichir(false);
    }

    Connections {
        target: app
        onApplicationActiveChanged: {
            if(app.applicationActive) {
                rafraichir(false);
            }
        }
    }

    Connections {
        target: FichierService
        onFin : {
            if(page.source !== source)  {
                // c'est pas pour nous
                return;
            }

            listModel.remplir(FichierService.statistique( page.source,false));
            occupation.taille = _etat.pourcentage *100;
        }
    }

    Connections {
        target: serv_config
        onStatChanged: {
            if(listModel.count===0) {
                return;
            }

            rafraichir(true);
        }
    }
}
