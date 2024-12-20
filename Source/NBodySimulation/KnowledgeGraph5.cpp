
#include "KnowledgeGraph.h"


#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


void AKnowledgeGraph::request_graph_http()
{
	TSharedPtr<FJsonObject> Js = MakeShareable(new FJsonObject());
	Js->SetStringField("some_field", "some_value");
	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Js.ToSharedRef(), JsonWriter);
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	// HttpRequest->SetVerb("POST");
	HttpRequest->SetVerb("GET");
	HttpRequest->SetHeader("Content-Type", "application/json");
	// FString URL = "https://www.space-track.org";
	// https://jsonplaceholder.typicode.com/todos/1
	// HttpRequest->SetURL("https://jsonplaceholder.typicode.com/todos/1");
	HttpRequest->SetURL("localhost:3062/api/v0/return_all_nodes111");
	// HttpRequest->SetContentAsString(OutputString)
	HttpRequest->OnProcessRequestComplete().BindUObject(
		this,
		&AKnowledgeGraph::request_graph_httpCompleted
	);
	HttpRequest->ProcessRequest();
	ll("YourFunction called", true, 0, TEXT("YourFunction: "));
}

void AKnowledgeGraph::request_a_graph()
{
	if (cgm == CGM::DATABASE)
	{
		request_graph_http();
	}
	else
	{

		if (cgm==CGM::JSON)
		{
				ll("hiiiiiiiii", true, 0, TEXT("YourFunction: "));
				const FString JsonFilePath = FPaths::ProjectContentDir() + "/data/state/" + fileIndexToPath[
					use_json_file_index];
				FString JsonString; //Json converted to FString
				FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
				JsonObject = MakeShareable(new FJsonObject());
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

				if (
					FJsonSerializer::Deserialize(JsonReader, JsonObject) &&
					JsonObject.IsValid())
				{
					TArray<TSharedPtr<FJsonValue>> jnodes = JsonObject->GetArrayField("nodes");
				}
				else
				{
					precheck_succeed = false;
					ll("Failed to deserialize JSON. ", true, 2);
					return;
				}
			
		}
		default_generate_graph_method();
	}
}

void AKnowledgeGraph::request_graph_httpCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	ll("request_graph_httpCompleted called", true, 0, TEXT("request_graph_httpCompleted: "));
	if (bWasSuccessful)
	{
		ll("Request was successful", true, 0, TEXT("request_graph_httpCompleted: "));
		if (
			// Content-Type: application/json; charset=utf-8
			Response->GetContentType() == "application/json" ||
			Response->GetContentType() == "application/json; charset=utf-8"
		)
		{
			JsonObject = MakeShareable(new FJsonObject());

			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(
				Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) &&
				JsonObject.IsValid())
			{
				// Process the JSON object here
				ll("Successfully parsed JSON.", true, 0, TEXT("request_graph_httpCompleted: "));
				//SomeOtherVariable = Js->GetStringField("nodes");

				// TArray<TSharedPtr<FJsonValue>> jnodes = Js->GetArrayField("nodes");
				// // jnodes.Num();
				// //
				// TArray<TSharedPtr<FJsonValue>> jedges = Js->GetArrayField("links");
				//
				// bool log = true;
				// ll("jedges.Num(): " + FString::FromInt(jedges.Num()), log);
				// int index = 0;
				// for (int32 i = 0; i < jedges.Num(); i++)
				// {
				// 	auto jobj = jedges[i]->AsObject();
				//
				// 	FString jid;
				// 	
				// 	FString jsourceS = jobj->GetStringField("source");
				// 	FString jtargetS = jobj->GetStringField("target");
				//
				//
				// 	ll("jsource: " + jsourceS + ", jtarget: " + jtargetS, log);
				// 	index++;
				// }


				default_generate_graph_method();
			}
			else
			{
				ll("Failed to parse JSON.", true, 2);
			}
		}
		else
			
		{
			ll("Response was not in JSON format.", true, 2);
			ll(FString::Printf(TEXT("Received Content-Type: %s"), *Response->GetContentType()), true, 2);
		}
	}
	else
	{
		debug_error_request(Request, Response);
	}
}


void AKnowledgeGraph::debug_error_request(FHttpRequestPtr Request, FHttpResponsePtr Response)
{
	precheck_succeed = false;
	ll("Request failed", true, 2);

	// Additional debugging information
	if (!Response.IsValid())
	{
		ll("No response was received.", true, 3);
	}
	else
	{
		ll(FString::Printf(TEXT("HTTP Status Code: %d"), Response->GetResponseCode()), true, 3);
		ll(FString::Printf(TEXT("Response Content: %s"), *Response->GetContentAsString()), true, 3);
	}

	
	// Log information about the request itself
	ll(FString::Printf(TEXT("HTTP Verb: %s"), *Request->GetVerb()), true, 3);
	ll(FString::Printf(TEXT("Requested URL: %s"), *Request->GetURL()), true, 3);

	// Checking connectivity or endpoint issues
	if (Response->GetResponseCode() == -1)
	{
		ll("Could be a network connectivity issue or the endpoint might be down.", true, 3);
	}
}