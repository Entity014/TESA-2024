from flask import Flask , request , jsonify , make_response

app = Flask(__name__)

@app.route("/")
def index():
    return "<p>Hello, World!</p>"

@app.route("/data",methods=["GET"])
def get_data():
    data={"key1":"value1" , "key2":"value2", "key3":"value3"}
    return jsonify(data),200

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=4000,debug=True)