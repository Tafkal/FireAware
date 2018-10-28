const defaultState = {
    danger: false
}

interface IState {
    danger: boolean
}
interface IProps {
    coords: {
        lat: number;
        long: number;
        marker: any;
    }
}

var url = "http://localhost:8080/data.json";

export const RegisteredDetectors:string[] = [];

export class Data {
    private  static _data: any;

    constructor() {
        Data._data={};
    }

    public static refresh = () => {
        console.log("refreshing");
        var jsonFile = new XMLHttpRequest();
            jsonFile.open("GET",url,true);
            jsonFile.send();

        jsonFile.onreadystatechange = () => {
        if (jsonFile.readyState== 4 && jsonFile.status == 200) {
            const newData = JSON.parse(jsonFile.responseText);
            for(var item in newData) {
                Data.registerReceiver(item, newData[item].props);
                Data.setState(item, newData[item].state);
            }
        }
     }
    }

    public static get data() {
        return Data._data;
    }

    public static setState(id: string, state: IState) {
        Data._data[id].state = state;
        if(state.danger) {
            addToMap(id)
        } else {
            removeFromMap(id);
        }
    }

    public static registerReceiver(id: string, props: IProps) {
        if(RegisteredDetectors.indexOf(id) === -1) {
            console.log("registered ", id)
            Data._data[id] = { props: props, state: defaultState};
            RegisteredDetectors.push(id);
        }
    }

    public static changeProps(id: string, props: IProps) {
        if(RegisteredDetectors.indexOf(id) !== -1) {
            Data._data[id] = { props: props, state: defaultState};
        }
    }
}


new Data();
setInterval(() => {Data.refresh();}, 5000);

import mapboxgl from 'mapbox-gl';

mapboxgl.accessToken = 'pk.eyJ1IjoiYW56dTMyNzgiLCJhIjoiY2puaHB2YnlkMGdoZjNrcGNmdTV1a241NiJ9.F8ki-9NPnNSn-LreY2XjyQ';
    var map = new mapboxgl.Map({
        container: 'map',
        style: 'mapbox://styles/mapbox/streets-v9'
    });

const addToMap = (name: string) => {
    if(!Data.data[name].marker) {
        console.log("added ", name)
        var marker = new mapboxgl.Marker({color: "#cc1010"}).setLngLat([Data.data[name].props.coords.long, Data.data[name].props.coords.lat]).addTo(map);
        Data.data[name].marker = marker;
    }
}
const removeFromMap = (name: string) => {
    if(Data.data[name].marker) {
        console.log("removed ", name)
        Data.data[name].marker.remove();
        Data.data[name].marker = null;
    }
}