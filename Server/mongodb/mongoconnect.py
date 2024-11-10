from pymongo.mongo_client import MongoCLient

uri = "mongodb+srv://user:pass@cluster0.2owdljr.mongodb.net/?retryWrites=true&w=majority"
#uri = "mongodb://myuser:mypassword@localhost:27017/mydb"

#Create a new client and connect to the server 
client = MongoCLient(uri)

#send a ping to confirm a successful connection
try:
    client.admin.command('ping')
    print("Pinged your deployment. You successfully connected to MongoDB! ")
    db = client["student"]
    collection  = db["std_info"]
    while True:
        print("===MENU===")
        print("1 : Show all records")
        print("2 : insert record")
        print("3 : update record")
        print("4 : delete record")
        print("5 : exit")
        choice  = input("Please select: ")
        choice = int(choice)
        if choice == 1:
            print(f'found {collection.count_document({})} record')
            all_student = collection.find()
            for std in all_student:
                print(std)
        elif choice == 2:
            id  = input("Input student id: ")
            name = input("Input fullname: ")
            major = input("Input major: ")
            gpa = input("gpa: ")
            gpa=float(gpa)
            try:
                collection.insert_one({"_id" : id,
                                       "fullname" : name,
                                       "major" : major,
                                       "gpa" : gpa})
            except Exception as e:
                print(e)
        elif choice == 3:
            student_id_to_update = input("Input the student ID to update: ")
            new_name = input("Input new full name: ")
            new_major = input("Input new major: ")
            new_gpa = input("Input new GPA: ")
            try:
                collection.update_one(
                    {"_id": student_id_to_update},
                    {"$set":
                        {
                            "name": new_name,
                            "major": new_major,
                            "gpa": new_gpa
                        }
                    }
                )
            except Exception as e:
                print(e)
        elif choice == 4:
            student_id_to_delete = input("Input the student ID to delete: ")
            try:
                collection.delete_one(
                    {"_id": student_id_to_delete}
                )
                print("Deleted")
            except Exception as e:
                print(e)
        elif choice == 5:
            break
except Exception as e:
    print(e)
finally:client.close()

            