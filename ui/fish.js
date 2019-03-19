//declare object garage and separate it from UI

var myParticleAccessToken = "dff12de210cd4eb5e5b02b0eba1d09b2aab99e1e"
var myDeviceId =            "1b002b000347363339343638"
var topic =                 "cse222/final_project"


function newLightEvent(objectContainingData) {
      var temp=JSON.parse(objectContainingData.data);
      console.log("state change");
      fish.temp=temp.temp;
      fish.light_color=temp.light_color;
      fish.alert=temp.alert;
      fish.stateChange();
}
var fish={

  //declare all the information needed
  temp:1,
  light_color:"",
  alert:false,
  feed_time:"",

  //set the listener to the function so that it can be used to monitor the change
  Listener:null,
  particle:null,

  update_state:function(state){
    console.log("call listener");
    this.Listener(state);
    if(this.Listener){
      this.Listener(state)
    }
    this.setup_stage=false;
  },
  stateChange: function () {
    var callingObject = this
    if(callingObject.Listener) {
      console.log("callingobject is set");
    var state = { temp:this.temp,
                  light_color:this.light_color,
                  alert:this.alert,
                  feed_time:this.feed_time};
    callingObject.update_state(state);
    }
  },
  //set listener to monitor the change
  setStateChangeListener: function(aListener) {
    console.log("set the listener");
    console.log(aListener);
    this.Listener = aListener;
  },
  stop_warm:function(){
    var functionData = {
             deviceId:myDeviceId,
             name: "stop_warm",
             argument: "",
             auth: myParticleAccessToken
        }
        function onSuccess(e) { console.log("stop_warm call success") }
        function onFailure(e) { console.log("stop_warm failed")
                               console.dir(e) }
        particle.callFunction(functionData).then(onSuccess,onFailure)
  },
  setup: function() {
    // Create a particle object
    particle = new Particle();
    this.setup_stage=true;
    // Get ready to subscribe to the event stream
    function onSuccess(stream) {
      // DONE:  This will "subscribe' to the stream and get the state"
      console.log("getEventStream success")
      stream.on('event', newLightEvent)
      var functionData = {
               deviceId:myDeviceId,
               name: "publishState",
               argument: "",
               auth: myParticleAccessToken
          }
          function onSuccess(e) { console.log("publishState call success") }
          function onFailure(e) { console.log("publishState failed")
                                 console.dir(e) }
          particle.callFunction(functionData).then(onSuccess,onFailure)
    }
    callingObject=this;
    function onFailure(e) { console.log("getEventStream call failed")
                            console.dir(e)}
    // Subscribe to the stream
    particle.getEventStream( { name: topic, auth: myParticleAccessToken, deviceId: 'mine' }).then(onSuccess, onFailure)
  }
}
