import QtQuick 2.2
import Sailfish.Silica 1.0
//import org.nemomobile.notifications 1.0
import Nemo.Notifications 1.0
import "../composents"
import "../hints"

Page {
    id:page

    property bool active: status===PageStatus.Active

    property bool showProcessusSysteme: serv_config.processusSysteme
    property int tailleMinProcessus : serv_config.tailleMinProcessus * 1024 * 1024

    property bool showFullMemory : true

    property string pageTitre : vi18n.get("memoire.titre")

    property int systemMemory : ProcessService.systemMemory

    property int classement : 0 // mem:0/cpu/alpha
    property bool menuOuvert: false
    property bool timerActif:true

    //-----------------------------------------
    function changementOrdre() {
        switch(classement) {
        case 0 :    // en utilisation memoire, passe en cpu
            classement = 1;
            pageTitre = vi18n.get("memoire.titre.cpu")
            rafraichir();
            break;
        case 1 :    // en charge cpu, passe en alpha
            classement = 2;
            pageTitre = vi18n.get("memoire.titre.alpha")
            rafraichir();
            break;
        default :   // retourne en mem
            classement = 0;
            pageTitre = vi18n.get("memoire.titre.mem")
            rafraichir();
        }
    }

    //-----------------------------------------
    function changementMemoire() {
        showFullMemory = showFullMemory ? false : true
        rafraichir()
    }

    //-----------------------------------------
    function changerRunner() {
        timerActif = timerActif ? false : true;
    }

    //-----------------------------------------
    function rafraichir() {

        var tmp = ProcessService.loadProcessList(false);
        listModel.clear();

        if(tmp.length>0) {
            listModel.remplir(tmp);
        }
    }

    //-----------------------------------------
    ListModel {
        id: listModel

        function remplir(l) {

            // liste temp
            var tmpL = [];
            for(var i in l) {
                if(showProcessusSysteme === false) {
                    if(l[i].user === "root" || l[i].user === "system") {
                        continue;
                    }
                }

                if(l[i].memory <= tailleMinProcessus) {
                    continue;
                }

                var ico=l[i].icon;
                if(ico.length===0) {
                    ico=l[i].user === "root" || l[i].user === "system" ? "image://theme/icon-m-developer-mode": "image://theme/icon-m-sailfish";
                }

                tmpL.push({
                              name: l[i].name,
                              icon: ico,
                              utilisateur: l[i].user,
                              opacite: 1.0,
                              tuer: ProcessService.inPurgatory(l[i].pid),
                              memory: l[i].memory,
                              memoryPercent: l[i].memory / systemMemory,
                              fullMemory: showFullMemory ? l[i].fullMemory : 0,
                              fullMemoryPercent: showFullMemory ? l[i].fullMemory / systemMemory : 0.0,
                              cpu: l[i].cpu,
                              percentType: classement == 1 ? "cpu" : "memoire",
                              type:l[i].type,
                              pid:l[i].pid
                          });
            }

            // classement
            switch(classement) {
            case 0 :
                if(showFullMemory)  {
                    tmpL=tmpL.sort(function(a,b) {
                        return b.fullMemory - a.fullMemory;
                    });
                    break;
                }

                tmpL=tmpL.sort(function(a,b) {
                    return b.memory - a.memory;
                });
                break;
            case 1 :
                tmpL=tmpL.sort(function(a,b) {
                    return b.cpu - a.cpu;
                });
                break;
            case 2 :
                // par nom
                tmpL=tmpL.sort(function(a,b) {
                    var ta = a.name.toLowerCase();
                    var tb = b.name.toLowerCase();
                    if(ta < tb) {return -1;}
                    if(ta > tb) {return 1;}
                    return 0;
                });
                break;
            }

            // remplissage / remplacement
            for(var i in tmpL) {
                if(get(i)) {
                    set(i, tmpL[i]);
                    continue;
                }

                append(tmpL[i]);
            }

            while(count > tmpL.length) {
                remove(count-1);
            }
        }
    }

    SilicaListView {
        id: fileList
        anchors.fill: parent
        contentWidth: parent.width
        clip: true
        spacing: Theme.paddingLarge

        //-----------------------------------------
        function actionMenu(ouvert) {
            menuOuvert=ouvert;
        }

        //-----------------------------------------
        function tuerProcess(nom, pid) {
            if(remorsepop.visible) {
                return;
            }

            (function() {
                var ppid=pid;
                remorsepop.execute(vi18n.get("memoire.arreterprocess").arg(nom), function(){

                    for(var i =0 ; i<listModel.count;i++) {
                        var el =listModel.get(i);
                        if(el.pid===ppid) {
                            if(el.tuer) {
                                return;
                            }

                            listModel.setProperty(i,"tuer", true);
                            ProcessService.kill(ppid);
                        }
                    }

                });
            })();
        }

        VerticalScrollDecorator { flickable:parent }

        PullDownMenu {
            MenuItem {
                text: vi18n.get("menu.config")
                onClicked: pageStack.push(Qt.resolvedUrl("config.qml"))
            }

            MenuItem {
                text: vi18n.get(showFullMemory ? "memoire.voirProcess" : "memoire.voirTout")
                onClicked: changementMemoire()
            }

            MenuItem {
                text: vi18n.get(classement === 0 ? "memoire.parCpu" : (classement === 1 ? "memoire.parAlpha" : "memoire.parMem"))
                onClicked: changementOrdre()
            }

            MenuItem {
                text: vi18n.get(timerActif ? "memoire.arretertimer" : "memoire.demarrertimer")
                onClicked: changerRunner()
            }
        }

        RemorsePopup { id: remorsepop }

        header: PageHeader {
            id:titre
            title: pageTitre
        }

        model: listModel

        delegate: RowMemoireDelegate{}
    }

    HintPress {
        id:hin1
        clee : "/vostok/memory/hint/processus"
        texte: vi18n.get("hint.process")
    }

    Timer {
        id: rechargement
        running: timerActif && active && menuOuvert===false
        repeat: true
        interval: 3500
        onTriggered:  {
            if(menuOuvert || active===false) {return;}
            ProcessService.loadProcessList(true);
        }
    }

    Notification {
        id: notification
        category: "x-sailfish.sailfish-utilities.error"
    }

    onActiveChanged: {
        if(active) {
            rafraichir();
        }
    }

    Component.onCompleted:  {
        hin1.lancer();
    }

    Connections {
        target: ProcessService

        onProcessListAvailable : {
            listModel.remplir(list);
        }

        onDoKill: {
            for(var i=0; i<listModel.count;i++) {
                if(listModel.get(i).pid === pid) {
                    listModel.setProperty(i,"tuer",true);
                    return;
                }
            }
        }

        onProcessKilled: {
            for(var i=0; i<listModel.count;i++) {
                if(listModel.get(i).pid === pid) {
                    notification.category="x-sailfish.sailfish-utilities.info";
                    notification.previewBody = vi18n.get("memoire.arretok").arg(listModel.get(i).name);
                    notification.close();
                    notification.publish();

                    listModel.remove(i);
                    return;
                }
            }
        }

        onProcessNOTKilled: {
            for(var i=0; i<listModel.count;i++) {
                if(listModel.get(i).pid === pid) {
                    listModel.setProperty(i,"tuer",false);

                    notification.category="x-sailfish.sailfish-utilities.error";
                    notification.previewBody = vi18n.get("memoire.arretPASok").arg(listModel.get(i).name);
                    notification.close();
                    notification.publish();
                    return;
                }
            }
        }
    }
}
