from flask import Flask, jsonify, request, send_file
from flask_jwt_extended import JWTManager, create_access_token, create_refresh_token, jwt_required, get_jwt_identity
from pymongo import MongoClient
from bson.objectid import ObjectId
import gridfs
import datetime
import io

app = Flask(__name__)
app.config["JWT_SECRET_KEY"] = "jwt_secret_key"
app.config["JWT_ACCESS_TOKEN_EXPIRES"] = datetime.timedelta(minutes=60)
app.config["JWT_REFRESH_TOKEN_EXPIRES"] = datetime.timedelta(days=30)
jwt = JWTManager(app)

# MongoDB setup
client = MongoClient("mongodb://password:username@localhost:27017/")
db = client["mydb"]
collection = db["data"]
fs = gridfs.GridFS(db, collection="matlabmodel")

# Authentication route
@app.route("/login", methods=["POST"])
def login():
    username = request.json.get("username", None)
    password = request.json.get("password", None)
    if username == "password" and password == "username":
        access_token = create_access_token(identity=username)
        refresh_token = create_refresh_token(identity=username)
        return jsonify(access_token=access_token, refresh_token=refresh_token), 200
    else:
        return jsonify({"msg": "Invalid credentials"}), 401

# Route to refresh the access token
@app.route("/refresh", methods=["POST"])
@jwt_required(refresh=True)
def refresh():
    current_user = get_jwt_identity()
    new_access_token = create_access_token(identity=current_user)
    return jsonify(access_token=new_access_token), 200

# POST: Upload a .zip file to the custom GridFS collection "actionbutton"
@app.route("/upload", methods=["POST"])
@jwt_required()
def upload_file():
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400

    file = request.files['file']
    print(request.files)
    
    if not file.filename.endswith('.onnx'):
        return jsonify({"error": "Only .zip files are allowed"}), 400

    # อัปโหลดไฟล์ไปยัง GridFS ที่มีคอลเลคชันเป็น "actionbutton"
    file_id = fs.put(file, filename=file.filename, content_type='application/zip')
    collection.insert_one({
        "file_name": file.filename,
        "file_id": file_id,
        "uploaded_by": get_jwt_identity(),
        "upload_date": datetime.datetime.utcnow()
    })
    

    return jsonify({"file_id": str(file_id)}), 201

# GET: Retrieve all data with only filenames
@app.route("/data", methods=["GET"])
@jwt_required()
def get_data():
    records = list(collection.find({}, {"file_name": 1}))
    for record in records:
        record["_id"] = str(record["_id"])
    return jsonify(records), 200

# GET: Retrieve data by file name
@app.route("/data/search", methods=["GET"])
@jwt_required()
def search_data_by_name():
    file_name = request.args.get("file_name")
    if not file_name:
        return jsonify({"error": "file_name parameter is required"}), 400

    records = list(collection.find({"file_name": file_name}))
    if not records:
        return jsonify({"error": "No matching data found"}), 404

    for record in records:
        record["_id"] = str(record["_id"])

    return jsonify(records), 200

# GET: Download a file using GridFS
@app.route("/data/<file_id>", methods=["GET"])
@jwt_required()
def download_file(file_id):
    try:
        file = fs.get(ObjectId(file_id))
        return send_file(
            io.BytesIO(file.read()),
            attachment_filename=file.filename,
            mimetype=file.content_type
        )
    except Exception:
        return jsonify({"error": "File not found"}), 404

# POST: Insert non-file data
@app.route("/data", methods=["POST"])
@jwt_required()
def insert_data():
    new_data = request.get_json()
    result = collection.insert_one(new_data)
    new_data["_id"] = str(result.inserted_id)
    return jsonify(new_data), 201

# PUT: Update non-file data by ID
@app.route("/data/<id>", methods=["PUT"])
@jwt_required()
def update_data(id):
    updated_data = request.get_json()
    result = collection.update_one({"_id": ObjectId(id)}, {"$set": updated_data})

    if result.matched_count == 0:
        return jsonify({"error": "Data not found"}), 404

    updated_data["_id"] = id
    return jsonify(updated_data), 200

# DELETE: Delete data and associated files by ID
@app.route("/data/<id>", methods=["DELETE"])
@jwt_required()
def delete_data(id):
    file_data = collection.find_one({"_id": ObjectId(id)})
    if "file_id" in file_data:
        fs.delete(ObjectId(file_data["file_id"]))

    result = collection.delete_one({"_id": ObjectId(id)})
    if result.deleted_count == 0:
        return jsonify({"error": "Data not found"}), 404

    return jsonify({"message": "Data deleted successfully"}), 200



#POST: log of prediction result
@app.route("/sent/log", methods=["POST"])
@jwt_required()
def sent_log():
    log_data = request.get_json()
    result = collection.insert_one(log_data)
    log_data["_id"] = str(result.inserted_id)
    return jsonify(log_data), 201
    
    
# Test route
@app.route("/")
def index():
    return "<h1>Let's go API!!!!!</h1>"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
