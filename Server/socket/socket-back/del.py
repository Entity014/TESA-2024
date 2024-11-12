from pymongo.mongo_client import MongoClient

uri = "mongodb://username:password@localhost:27017/"  # Update with actual credentials if needed

# Create a new client and connect to the server
try:
    client = MongoClient(uri)
    print("Connecting to MongoDB...")

    # Send a ping to confirm a successful connection
    client.admin.command('ping')
    print("Pinged your deployment. You successfully connected to MongoDB!")

    db = client["student"]
    collection = db["std_info"]
    
    while True:
        print("===MENU===")
        print("1 : Show all records")
        print("2 : insert record")
        print("3 : update record")
        print("4 : delete record")
        print("5 : exit")
        choice = input("Please select: ")
        
        try:
            choice = int(choice)
        except ValueError:
            print("Invalid input. Please enter a number.")
            continue

        if choice == 1:
            print(f'Found {collection.count_documents({})} records')
            all_student = collection.find()
            for std in all_student:
                print(std)
                
        elif choice == 2:
            id = input("Number: ")
            name = input("Input fullname: ")
            major = input("Input major: ")
            gpa = float(input("gpa: "))
            try:
                collection.insert_one({"_id": id, "fullname": name, "major": major, "gpa": gpa})
                print("Record inserted successfully.")
            except Exception as e:
                print("Error inserting record:", e)

        elif choice == 3:
            student_id_to_update = input("Input the student ID to update: ")
            new_name = input("Input new full name: ")
            new_major = input("Input new major: ")
            new_gpa = float(input("Input new GPA: "))
            try:
                collection.update_one({"_id": student_id_to_update},
                                      {"$set": {"fullname": new_name, "major": new_major, "gpa": new_gpa}})
                print("Record updated successfully.")
            except Exception as e:
                print("Error updating record:", e)

        elif choice == 4:
            student_id_to_delete = input("Input the student ID to delete: ")
            try:
                collection.delete_one({"_id": student_id_to_delete})
                print("Record deleted.")
            except Exception as e:
                print("Error deleting record:", e)

        elif choice == 5:
            print("Exiting program.")
            break
        else:
            print("Invalid selection. Please choose a number between 1 and 5.")

except Exception as e:
    print("Error connecting to MongoDB:", e)
finally:
    client.close()
    print("Connection closed.")
